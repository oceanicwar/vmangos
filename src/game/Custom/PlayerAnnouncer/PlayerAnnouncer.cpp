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

    char message[256 + 32];
    sprintf(message, format.c_str(), playerName);

    ActionScriptHelper::AnnounceToAll(message, player);
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

    char message[256 + 32];
    sprintf(message, format.c_str(), playerName);

    ActionScriptHelper::AnnounceToAll(message, player);
}
