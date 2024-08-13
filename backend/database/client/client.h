#pragma once

#include <drogon/orm/DbClient.h>
#include <memory>

namespace backend
{
    std::shared_ptr<drogon::orm::DbClient> getDbClient();
}