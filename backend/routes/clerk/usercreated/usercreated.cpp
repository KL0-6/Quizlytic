#include "usercreated.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "../../../../backend/database/client/client.h"

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
    nlohmann::json jsonData = nlohmann::json::parse(request->getBody());

    if (jsonData.is_discarded()) 
        return callback(generateError("Invalid JSON"));

    try 
    {
        if (!jsonData.contains("data") || !jsonData.at("data").contains("id"))      
            return callback(generateError("Missing fields in request body!"));

        nlohmann::json& data = jsonData.at("data");

        std::string userId = data.at("id").get<std::string>();
        std::string email = data.at("email_addresses").at(0).at("email_address").get<std::string>();
        
        const auto& dbClient = backend::getDbClient();
        dbClient->execSqlSync("INSERT INTO users (email, clerkUserId) VALUES (?, ?)", email, userId);

        Json::Value returnValue;
        returnValue["status"] = "OK!";

        HttpResponsePtr response = HttpResponse::newHttpJsonResponse(returnValue);
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        response->setStatusCode(drogon::HttpStatusCode::k200OK);

        return callback(response);
    }
    catch (const std::exception &e) 
    {
        return callback(generateError(e.what()));
    }

    return callback(generateError("Unknown error contacting API!"));
}