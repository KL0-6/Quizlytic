#include "chat.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "../../database/client/client.h"

std::string apiKey = "";
std::string endpoint = "https://api.groq.com/openai/v1/chat/completions"; // Updated to match your example

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

    // ERROR: No body, or invalid body
    if(!body || body->isNull() || !body->isMember("data") || !body->isMember("clerkUserId"))
        return callback(generateError("Invalid JSON Body!"));

    std::string userInput = body->get("data", "").asString();
    std::string clerkUserId = body->get("clerkUserId", "").asString();

    if(userInput.empty() || clerkUserId.empty())
        return callback(generateError("Input or ID is empty!"));

    try
    {
        const auto& dbClient = backend::getDbClient();
        drogon::orm::Result result = dbClient->execSqlSync("SELECT * FROM users WHERE clerkUserId = ?", clerkUserId);

        if (result.empty())
            return callback(generateError("clerkUserId not found in the database!", drogon::HttpStatusCode::k404NotFound));
    }
    catch(const std::exception& e)
    {
        return callback(generateError("Error connecting to database!"));
    }
    
    std::string payload = R"({
        "max_tokens": 1024,
        "messages": [
            {
                "content": "You are an AI assistant specifically designed to generate educational flashcards based on user requests. Each flashcard should be formatted as: [ { question: \"General concept or fact\", answer: \"Explanation or definition\" } ]. Provide a maximum of 15 flashcards per request. If a number of flashcards isn't provided, ONLY send 5 flashcards. Your responses must strictly adhere to the educational topic requested and avoid any inappropriate or NSFW content. Return only the JSON array of flashcards. Do not include any additional text, explanations, or content outside of the JSON format.",
                "role": "system"
            },
            {
                "content": ")" + userInput + R"(",
                "role": "user"
            }
        ],
        "model": "llama3-8b-8192",
        "stop": null,
        "stream": false,
        "temperature": 0.7,
        "top_p": 0.9
    })";

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

            std::string responseText = "No valid response from the API.";

            if (jsonResponse.contains("choices") && jsonResponse["choices"].is_array() && !jsonResponse["choices"].empty())
                responseText = jsonResponse["choices"][0]["message"]["content"].get<std::string>();

            HttpResponsePtr returnResponse = HttpResponse::newHttpResponse();
            returnResponse->addHeader("Access-Control-Allow-Origin", "*");
            returnResponse->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
            returnResponse->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");

            returnResponse->setBody(responseText);

            return callback(returnResponse);
        } 
    }
    catch(const std::exception& e)
    {
        return callback(generateError("Request to API failed!"));
    }
    
    return callback(generateError("Unknown error contacting API!"));
}