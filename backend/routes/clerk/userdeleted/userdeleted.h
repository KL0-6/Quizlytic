#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class ClerkUserDeletedCtrl : public drogon::HttpSimpleController<ClerkUserDeletedCtrl>
{
    public:
        HttpResponsePtr generateError(const char* const, HttpStatusCode = drogon::HttpStatusCode::k400BadRequest);
        void asyncHandleHttpRequest(const HttpRequestPtr&, std::function<void(const HttpResponsePtr &)> &&) override;
        PATH_LIST_BEGIN
        PATH_ADD("/clerk/userdeleted", Post, Options);
        PATH_LIST_END
};