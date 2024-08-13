#include "test.h"

void TestCtrl::asyncHandleHttpRequest(const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> && callback)
{
    Json::Value ret;
    ret["test"] = "Hello!";

    const auto response = HttpResponse::newHttpJsonResponse(std::move(ret));

    callback(response);
}