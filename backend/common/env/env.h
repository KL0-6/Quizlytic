#pragma once
#include <nlohmann/json.hpp>
#include <filesystem>
#include <optional>
#include <iostream>
#include <fstream>
#include <memory>

namespace env
{
    bool setEnvPath(std::string);

    std::shared_ptr<nlohmann::json> getEnv();

    template<typename T>
    std::optional<T> getEnvValue(const std::string& key)
    {
        const auto& envInstance = getEnv();

        if (!envInstance)
            throw std::runtime_error("envInstance is not initialized. Call 'setEnvPath' before 'getEnvValue'");

        if(!envInstance->contains(key))
            return std::nullopt;

        try
        {
            return envInstance->at(key).get<T>();
        }
        catch (const nlohmann::json::type_error& e)
        {
            std::cerr << "Type error: " << e.what() << std::endl;
            return std::nullopt;
        }
    }
}