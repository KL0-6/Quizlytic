#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class ClerkUserCreatedCtrl : public drogon::HttpSimpleController<ClerkUserCreatedCtrl>
{
    public:
        HttpResponsePtr generateError(const char* const, HttpStatusCode = drogon::HttpStatusCode::k400BadRequest);
        void asyncHandleHttpRequest(const HttpRequestPtr&, std::function<void(const HttpResponsePtr &)> &&) override;
        PATH_LIST_BEGIN
        PATH_ADD("/clerk/usercreated", Post, Options);
        PATH_LIST_END
};