#include "userdeleted.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

HttpResponsePtr ClerkUserDeletedCtrl::generateError(const char* const message, HttpStatusCode statusCode)
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

void ClerkUserDeletedCtrl::asyncHandleHttpRequest(const HttpRequestPtr& request, std::function<void(const HttpResponsePtr &)> && callback)
{          
    nlohmann::json jsonData;
    try 
    {
        jsonData = nlohmann::json::parse(request->getBody());

        if (!jsonData.contains("data") || !jsonData.at("data").contains("id"))      
            return callback(generateError("Missing fields in request body!", drogon::k400BadRequest));
    } 
    catch (const nlohmann::json::parse_error& e) 
    {
        return callback(generateError("Invalid JSON", drogon::k400BadRequest));
    }

    try 
    {
        nlohmann::json& data = jsonData.at("data");

        std::string userId = data.at("id").get<std::string>();

        std::shared_ptr<drogon::orm::DbClient> dbClient = drogon::app().getDbClient();

        dbClient->execSqlAsync("DELETE FROM flashcards WHERE clerkUserId = ?", [this, userId, dbClient, callback](const drogon::orm::Result& result) 
        {
            dbClient->execSqlAsync("DELETE FROM users WHERE clerkUserId = ?", [this, callback](const drogon::orm::Result& result) 
            {
                Json::Value returnValue;
                returnValue["status"] = "User and associated flashcards deleted successfully!";

                HttpResponsePtr response = HttpResponse::newHttpJsonResponse(returnValue);
                response->addHeader("Access-Control-Allow-Origin", "*");
                response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
                response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
                response->setStatusCode(drogon::HttpStatusCode::k200OK);

                callback(response);
            },
            [this, callback](const drogon::orm::DrogonDbException& e) 
            {
                callback(generateError("Database error while deleting user!", drogon::HttpStatusCode::k500InternalServerError));
            },
            userId);
        },
        [this, callback](const drogon::orm::DrogonDbException& e) 
        {
            callback(generateError("Database error while deleting flashcards!", drogon::HttpStatusCode::k500InternalServerError));
        },
        userId);
    }
    catch (const std::exception &e) 
    {
        return callback(generateError(e.what(), drogon::k500InternalServerError));
    }
}
