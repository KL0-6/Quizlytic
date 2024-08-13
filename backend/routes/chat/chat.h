#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class ChatCtrl : public drogon::HttpSimpleController<ChatCtrl>
{
    public:
        static void setApiKey(std::string);
        void asyncHandleHttpRequest(const HttpRequestPtr&, std::function<void(const HttpResponsePtr &)> &&) override;
        PATH_LIST_BEGIN
        PATH_ADD("/chat", Post, Options);
        PATH_LIST_END
};