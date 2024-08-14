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

    drogon::app().loadConfigFile(envPath).addListener(drogonIp.value(), drogonPort.value()).setThreadNum(drogonThreads.value()).run();

    return 0;
}
