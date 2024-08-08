#include "ActionScriptConfig.h"

template <>
const std::string ActionScriptConfig::GetValue<std::string>(const char* key)
{
    auto value = GetValueInternal(key);
    if (!value)
    {
        return "";
    }

    return *value;
}

template <>
const uint32 ActionScriptConfig::GetValue<uint32>(const char* key)
{
    auto value = GetValueInternal(key);
    if (!value)
    {
        return 0;
    }

    auto result = static_cast<uint32>(strtoul(value->c_str(), nullptr, 10));
    if (!result)
    {
        return 0;
    }

    return result;
}

template <>
const bool ActionScriptConfig::GetValue<bool>(const char* key)
{
    auto value = GetValueInternal(key);
    if (!value)
    {
        return false;
    }

    auto result = strtoul(value->c_str(), nullptr, 10) > 0;
    if (!result)
    {
        return false;
    }

    return result;
}

const std::string* ActionScriptConfig::GetValueInternal(const char* key)
{
    auto it = config.find(key);
    if (it == config.end())
    {
        sLog.Out(LogType::LOG_BASIC, LogLevel::LOG_LVL_ERROR, "No config key '%s' was found for action script '%s'.", key, name);
        return nullptr;
    }

    return &it->second;
}
