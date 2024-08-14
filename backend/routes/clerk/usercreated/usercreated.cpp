#include "usercreated.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

HttpResponsePtr ClerkUserCreatedCtrl::generateError(const char* const message, HttpStatusCode statusCode)
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

void ClerkUserCreatedCtrl::asyncHandleHttpRequest(const HttpRequestPtr& request, std::function<void(const HttpResponsePtr &)> && callback)
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
        std::string email = data.at("email_addresses").at(0).at("email_address").get<std::string>();

        std::shared_ptr<drogon::orm::DbClient> dbClient = drogon::app().getDbClient();

        dbClient->execSqlAsync("INSERT INTO users (email, clerkUserId) VALUES (?, ?)", [this, callback](const drogon::orm::Result& result) 
        {
            Json::Value returnValue;
            returnValue["status"] = "OK!";

            HttpResponsePtr response = HttpResponse::newHttpJsonResponse(returnValue);
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
            response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
            response->setStatusCode(drogon::HttpStatusCode::k200OK);

            callback(response);
        },
        [this, callback](const drogon::orm::DrogonDbException& e) 
        {
            callback(generateError("Database error while inserting user!", drogon::HttpStatusCode::k500InternalServerError));
        },
        email, userId
        );
    }
    catch (const std::exception &e) 
    {
        return callback(generateError(e.what(), drogon::k500InternalServerError));
    }
}
