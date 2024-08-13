#include "database/client/client.h"
#include "routes/chat/chat.h"
#include "common/env/env.h"
#include <drogon/drogon.h>

int main(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        LOG_FATAL << "Provide env.json path as first argument!";

        return 0;
    }

    std::string envPath = argv[1];
    if(!env::setEnvPath(envPath))
    {
        LOG_FATAL << envPath << " doesn't exist or isn't readable!";

        return 0;
    }

    const auto& env = env::getEnv();

    std::optional<std::string> drogonIp = env::getEnvValue<std::string>("drogonIp");
    std::optional<int> drogonPort = env::getEnvValue<int>("drogonPort");
    std::optional<int> drogonThreads = env::getEnvValue<int>("drogonThreads");
    std::optional<std::string> groqKey = env::getEnvValue<std::string>("groqKey");
    
    if(!drogonIp.has_value() || !drogonPort.has_value()  || !drogonThreads.has_value() || !groqKey.has_value())
    {
        LOG_FATAL << "Please check your env json. Required values are missing.";

        return 0;
    }

    ChatCtrl::setApiKey(groqKey.value());

    LOG_INFO << "Server running on " << drogonIp.value() << ":" << drogonPort.value();

    const auto& dbClient = backend::getDbClient();
    
    try
    {
        dbClient->execSqlSync("USE flashcards;");
    }
    catch (const drogon::orm::DrogonDbException &e)
    {   
        LOG_ERROR << "Failed to connect to testdb!";

        return 0;
    }

    dbClient->execSqlAsync("SELECT * FROM users WHERE user_hash = ?", 
        [](const drogon::orm::Result &result)
        {
            if (result.size() > 0)
            {
                std::cout << "User found!" << std::endl;
                for (const auto &row : result)
                {
                    std::cout << "User ID: " << row["user_id"].as<std::string>() << std::endl;
                    std::cout << "User Hash: " << row["user_hash"].as<std::string>() << std::endl;
                    std::cout << "Is Premium: " << row["is_premium"].as<int>() << std::endl;
                    std::cout << "Created At: " << row["created_at"].as<std::string>() << std::endl;
                }
            }
            else
            {
                std::cout << "No user found with hash: " << std::endl;
            }
        },
        [](const drogon::orm::DrogonDbException &e)
        {
            std::cerr << "Async error: " << e.base().what() << std::endl;
        }, 
        "user1_hash"
    );

    drogon::app().addListener(drogonIp.value(), drogonPort.value()).setThreadNum(drogonThreads.value()).run();

    return 0;
}
