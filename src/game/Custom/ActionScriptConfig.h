#ifndef _ACTION_SCRIPT_CONFIG_H
#define _ACTION_SCRIPT_CONFIG_H

class ActionScriptConfig
{
public:
    ActionScriptConfig() { }
    ActionScriptConfig(std::string name, std::unordered_map<std::string, std::string> map) : config(map), name(name) { }

    template <typename T>
    const T GetValue(const char* key);

private:
    const std::string* GetValueInternal(const char* key);

private:
    std::string name;
    std::unordered_map<std::string, std::string> config;
};

#endif
