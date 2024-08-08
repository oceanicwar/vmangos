#include "PlayerAnnouncer.h"

#include "ActionScriptHelper.h"

#include "Chat.h"

void PlayerAnnouncerScript::OnPlayerLogin(Player* player, bool firstLogin)
{
    if (!player)
    {
        return;
    }

    char playerName[32];
    sprintf(playerName, "%s%s", ActionScriptHelper::GetPlayerClassColor(player), player->GetName());

    auto format = this->GetConfig()->GetValue<std::string>(firstLogin ? "FirstLogin.Format" : "Login.Format");

    auto securityLevel = player->GetSession()->GetSecurity();

    if (this->GetConfig()->GetValue<bool>("Login.AnnounceStaff") &&
        securityLevel > 1)
    {
        format = this->GetConfig()->GetValue<std::string>("Login.AnnounceStaff.Format");

        if (!ActionScriptHelper::ReplaceString(format, "$security", ActionScriptHelper::GetSecurityName(securityLevel)))
        {
            sLog.Out(LogType::LOG_BASIC, LogLevel::LOG_LVL_ERROR, "$security was not found in 'Login.AnnounceStaff.Format'.");
        }
    }

    if (!ActionScriptHelper::ReplaceString(format, "$name", player->GetName()))
    {
        sLog.Out(LogType::LOG_BASIC, LogLevel::LOG_LVL_ERROR, "$name was not found in 'Login.AnnounceStaff.Format'.");
    }

    ActionScriptHelper::AnnounceToAll(format.c_str(), player);
}

void PlayerAnnouncerScript::OnPlayerLogout(Player* player)
{
    if (!player)
    {
        return;
    }

    char playerName[32];
    sprintf(playerName, "%s%s", ActionScriptHelper::GetPlayerClassColor(player), player->GetName());

    auto format = this->GetConfig()->GetValue<std::string>("Logout.Format");

    auto securityLevel = player->GetSession()->GetSecurity();

    if (this->GetConfig()->GetValue<bool>("Logout.AnnounceStaff") &&
        securityLevel > 1)
    {
        format = this->GetConfig()->GetValue<std::string>("Logout.AnnounceStaff.Format");

        if (!ActionScriptHelper::ReplaceString(format, "$security", ActionScriptHelper::GetSecurityName(securityLevel)))
        {
            sLog.Out(LogType::LOG_BASIC, LogLevel::LOG_LVL_ERROR, "$security was not found in 'Logout.AnnounceStaff.Format'.");
        }
    }

    if (!ActionScriptHelper::ReplaceString(format, "$name", player->GetName()))
    {
        sLog.Out(LogType::LOG_BASIC, LogLevel::LOG_LVL_ERROR, "$name was not found in 'Logout.AnnounceStaff.Format'.");
    }

    ActionScriptHelper::AnnounceToAll(format.c_str(), player);
}
