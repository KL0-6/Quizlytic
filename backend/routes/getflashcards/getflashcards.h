#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class GetFlashcardsCtrl : public drogon::HttpSimpleController<GetFlashcardsCtrl>
{
    public:
        HttpResponsePtr generateError(const char* const, HttpStatusCode = drogon::HttpStatusCode::k400BadRequest);
        void asyncHandleHttpRequest(const HttpRequestPtr&, std::function<void(const HttpResponsePtr &)> &&) override;
        PATH_LIST_BEGIN
        PATH_ADD("/getflashcards", Get, Options);
        PATH_LIST_END
};