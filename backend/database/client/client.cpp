#include "client.h"
#include "../../common/env/env.h"

static std::shared_ptr<drogon::orm::DbClient> dbClientInstance;

std::shared_ptr<drogon::orm::DbClient> backend::getDbClient() 
{
    if (!dbClientInstance) 
    {
        std::optional<nlohmann::json> dbConfigOpt = env::getEnvValue<nlohmann::json>("db");
        
        if (!dbConfigOpt || !dbConfigOpt->is_object())
            throw std::runtime_error("Database configuration is missing or not an object.");

        const nlohmann::json& dbConfig = dbConfigOpt.value();

        std::string dbHost = dbConfig.value("host", "127.0.0.1");
        int dbPort = dbConfig.value("port", 3306);
        std::string dbUser = dbConfig.value("username", "root");
        std::string dbPassword = dbConfig.value("password", "");
        std::string dbName = dbConfig.value("dbname", "");
        std::size_t connNum = dbConfig.value("connNum", 4);

        if (dbHost.empty() || dbUser.empty() || dbPassword.empty())
            throw std::runtime_error("Critical database configuration values are missing.");

        std::string connInfo = "host=" + dbHost + ";port=" + std::to_string(dbPort) + ";dbname=" + dbName + ";user=" + dbUser + ";password=" + dbPassword;     

        dbClientInstance = drogon::orm::DbClient::newMysqlClient(connInfo, connNum);
    }

    return dbClientInstance;
}