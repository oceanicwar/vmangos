#include "PlayerAnnouncer.h"

#include "ActionScriptHelper.h"

#include "Chat.h"

void PlayerAnnouncerScript::OnPlayerLogin(Player* player)
{
    if (!player)
    {
        return;
    }

    char message[200];
    sprintf(message, "|cff00FF00[ |cffFFFFFFWorld |cff00FF00]: |cffFFFFFFPlayer %s%s |cffFFFFFFhas entered the world.|r", ActionScriptHelper::GetPlayerClassColor(player), player->GetName());
    ActionScriptHelper::AnnounceToAll(message, player);
}

void PlayerAnnouncerScript::OnPlayerLogout(Player* player)
{
    if (!player)
    {
        return;
    }

    char message[200];
    sprintf(message, "|cffFF0000[ |cffFFFFFFWorld |cffFF0000]: |cffFFFFFFPlayer %s%s |cffFFFFFFhas left the world.|r", ActionScriptHelper::GetPlayerClassColor(player), player->GetName());
    ActionScriptHelper::AnnounceToAll(message, player);
}