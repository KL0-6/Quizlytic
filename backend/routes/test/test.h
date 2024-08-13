#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class TestCtrl : public drogon::HttpSimpleController<TestCtrl>
{
    public:
        void asyncHandleHttpRequest(const HttpRequestPtr&, std::function<void(const HttpResponsePtr &)> &&) override;
        PATH_LIST_BEGIN
        PATH_ADD("/test", Get);
        PATH_LIST_END
};