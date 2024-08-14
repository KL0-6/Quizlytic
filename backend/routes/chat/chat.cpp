#include "chat.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "../../common/base64/base64.h"
#include "../../common/cryptography/cryptography.h"

std::string apiKey = "";
std::string endpoint = "https://api.groq.com/openai/v1/chat/completions";

void ChatCtrl::setApiKey(std::string _apiKey) 
{
    apiKey = _apiKey;
}

HttpResponsePtr generateError(const char* const message, HttpStatusCode statusCode = drogon::HttpStatusCode::k400BadRequest)
{
    Json::Value error;
    error["error"] = message;

    HttpResponsePtr response = HttpResponse::newHttpJsonResponse(error);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    
    response->setStatusCode(statusCode);

    return response;
}

void ChatCtrl::asyncHandleHttpRequest(const HttpRequestPtr& request, std::function<void(const HttpResponsePtr &)> && callback)
{
    if (request->method() == HttpMethod::Options)
    {
        HttpResponsePtr response = HttpResponse::newHttpResponse();        
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");

        response->setStatusCode(drogon::HttpStatusCode::k200OK);

        return callback(response);
    }

    std::shared_ptr<Json::Value> body = request->getJsonObject();

    if(!body || body->isNull() || !body->isMember("data") || !body->isMember("clerkUserId"))
        return callback(generateError("Invalid JSON Body!"));

    std::string title = body->get("title", "").asString();
    std::string difficulty = body->get("difficulty", "").asString();
    std::string description = body->get("data", "").asString();
    std::string clerkUserId = body->get("clerkUserId", "").asString();

    if(title.empty() || description.empty() || difficulty.empty() || clerkUserId.empty())
        return callback(generateError("Input or ID is empty!"));

    std::shared_ptr<drogon::orm::DbClient> dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync("SELECT * FROM users WHERE clerkUserId = ?", 
    [this, dbClient, title, difficulty, description, clerkUserId, callback](const drogon::orm::Result &result) 
    {
        if (result.empty()) 
            return callback(generateError("clerkUserId not found in the database!", drogon::HttpStatusCode::k404NotFound));

        std::string payload = R"({
            "max_tokens": 1024,
            "messages": [
                {
                    "content": "You are an AI assistant specifically designed to generate educational flashcards based on user requests. Each flashcard should be formatted as: [ { question: \"General concept or fact\", answer: \"Explanation or definition\" } ]. Remember, you are generating flashcards. Avoid specific questions, generate content that is broad and conceptual. Provide a maximum of 15 flashcards per request. If the user does not specify a number of flashcards, generate exactly 5 flashcards by default. Your responses must strictly adhere to the educational topic requested and avoid any inappropriate or NSFW content. Return only the JSON array of flashcards. Do not include any additional text, explanations, or content outside of the JSON format.",
                    "role": "system"
                },
                {
                    "content": ")" + description + " with a difficulty of " + difficulty + R"(",
                    "role": "user"
                }
            ],
            "model": "llama3-8b-8192",
            "stop": null,
            "stream": false,
            "temperature": 0.7,
            "top_p": 0.9
        })";

        drogon::app().getLoop()->queueInLoop([this, dbClient, payload, title, difficulty, description, clerkUserId, callback]() 
        {
            try 
            {
                cpr::Response apiResponse = cpr::Post(
                    cpr::Url{endpoint},
                    cpr::Header{{"Authorization", "Bearer " + apiKey}, {"Content-Type", "application/json"}},
                    cpr::Body{payload}
                );

                if (apiResponse.status_code == 200) 
                {
                    auto jsonResponse = nlohmann::json::parse(apiResponse.text);

                    if (jsonResponse.contains("choices") && jsonResponse["choices"].is_array() && !jsonResponse["choices"].empty())
                    {
                        std::string responseText = jsonResponse["choices"][0]["message"]["content"].get<std::string>();

                        const std::string& datab64 = base64::encode(responseText, responseText.size());
                        const std::string& dataHash = cryptography::hash(datab64.c_str(), datab64.size());

                        dbClient->execSqlAsync("INSERT INTO flashcards (setHash, title, description, difficulty, data, clerkUserId) VALUES (?, ?, ?, ?, ?, ?)", 
                        [callback, dataHash, responseText](const drogon::orm::Result &result) 
                        {
                            Json::Value returnData;
                            returnData["hash"] = dataHash;
                            returnData["data"] = responseText;

                            HttpResponsePtr returnResponse = HttpResponse::newHttpJsonResponse(returnData);
                            returnResponse->addHeader("Access-Control-Allow-Origin", "*");
                            returnResponse->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
                            returnResponse->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
                            returnResponse->setStatusCode(drogon::HttpStatusCode::k200OK);

                            callback(returnResponse);
                        }, 
                        [callback](const drogon::orm::DrogonDbException &e) 
                        {
                            callback(generateError("Database error while inserting flashcards!", drogon::HttpStatusCode::k500InternalServerError));
                        },
                        dataHash, title, description, difficulty, datab64, clerkUserId
                        );
                    } 
                    else 
                    {
                        callback(generateError("No valid response from the API!", drogon::HttpStatusCode::k500InternalServerError));
                    }
                } 
                else 
                {
                    callback(generateError("Failed to contact the API!", drogon::HttpStatusCode::k500InternalServerError));
                }
            } 
            catch (const std::exception &e) 
            {
                callback(generateError("Request to API failed!", drogon::HttpStatusCode::k500InternalServerError));
            }
        });
    },
    [callback](const drogon::orm::DrogonDbException &e) 
    {
        callback(generateError("Error connecting to database!", drogon::HttpStatusCode::k500InternalServerError));
    }, clerkUserId);
}
