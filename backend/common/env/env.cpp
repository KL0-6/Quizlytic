#include "env.h"

static std::shared_ptr<nlohmann::json> envInstance;
static std::string envData = "";

std::string readfile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (file.good())
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return "";
}

bool env::setEnvPath(std::string path)
{
    if(!std::filesystem::exists(path))
        return false;

    envData = readfile(path);
    if(envData.empty())
        return false;

    return true;
}

std::shared_ptr<nlohmann::json> env::getEnv()
{
    if(!envInstance)
    {
        if (envData.empty()) 
            throw std::runtime_error("envData is not defined. Call 'setEnvPath' before 'getEnv'");

        nlohmann::json jsonData = nlohmann::json::parse(envData);

        if (!jsonData.contains("env"))
            throw std::runtime_error("envData is not formatted correctly!");

        envInstance = std::make_shared<nlohmann::json>(jsonData.at("env"));
    }

    return envInstance;
}