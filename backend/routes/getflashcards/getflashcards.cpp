#include "getflashcards.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "../../common/base64/base64.h"
#include "../../common/cryptography/cryptography.h"

HttpResponsePtr GetFlashcardsCtrl::generateError(const char* const message, HttpStatusCode statusCode)
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

void GetFlashcardsCtrl::asyncHandleHttpRequest(const HttpRequestPtr& request, std::function<void(const HttpResponsePtr &)> && callback)
{
    try
    {
        const drogon::SafeStringMap<std::string>& queryParams = request->getParameters();
        auto userIdIt = queryParams.find("userId");

        if (userIdIt == queryParams.end() || userIdIt->second.empty())
            return callback(generateError("Missing or invalid userId", drogon::k400BadRequest));

        std::string userId = userIdIt->second;

        std::shared_ptr<drogon::orm::DbClient> dbClient = drogon::app().getDbClient();

        dbClient->execSqlAsync(
            "SELECT title, description, setHash, data FROM flashcards WHERE clerkUserId = ?",
            [this, callback](const drogon::orm::Result& result)
            {
                if (result.empty())
                    return callback(generateError("No flashcards found for the user", drogon::k404NotFound));

                nlohmann::json flashcards = nlohmann::json::array();
                for (const auto& row : result)
                {
                    std::string title = row["title"].as<std::string>();
                    std::string description = row["description"].as<std::string>();
                    std::string setHash = row["setHash"].as<std::string>();
                    std::string encodedData = row["data"].as<std::string>();
                    std::string decodedData = drogon::utils::base64Decode(encodedData);

                    nlohmann::json flashcard;
                    flashcard["title"] = title;
                    flashcard["description"] = description;
                    flashcard["hash"] = setHash;
                    flashcard["data"] = decodedData;

                    flashcards.push_back(flashcard);
                }

                // Convert nlohmann::json to Json::Value for Drogon
                Json::Value jsonResponse;
                std::stringstream ss(flashcards.dump());
                ss >> jsonResponse;

                HttpResponsePtr response = HttpResponse::newHttpJsonResponse(jsonResponse);
                response->addHeader("Access-Control-Allow-Origin", "*");
                response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
                response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");

                callback(response);
            },
            [this, callback](const drogon::orm::DrogonDbException& e)
            {
                callback(generateError(e.base().what(), drogon::k500InternalServerError));
            },
            userId
        );
    }
    catch (const std::exception &e)
    {
        return callback(generateError(e.what(), drogon::k500InternalServerError));
    }
}
