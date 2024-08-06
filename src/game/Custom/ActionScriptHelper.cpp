#include "ActionScriptHelper.h"

#include "Chat.h"
#include "Player.h"

const char* ActionScriptHelper::GetPlayerClassColor(Player* player)
{
    switch (player->GetClass())
    {
    case CLASS_WARRIOR:
        return "|cffC79D6E";
        break;
    case CLASS_PALADIN:
        return "|cffF68CB9";
        break;
    case CLASS_HUNTER:
        return "|cffABD474";
        break;
    case CLASS_ROGUE:
        return "|cffFEF569";
        break;
    case CLASS_PRIEST:
        return "|cffFFFFFF";
        break;
    case CLASS_SHAMAN:
        return "|cff0070DE";
        break;
    case CLASS_MAGE:
        return "|cff40C7EC";
        break;
    case CLASS_WARLOCK:
        return "|cffC79D6E";
        break;
    case CLASS_DRUID:
        return "|cffFF7E0A";
        break;
    }

    return "|r";
}

void ActionScriptHelper::AnnounceToAll(const char* message, Player* ignorePlayer /* = nullptr */)
{
    const auto& sessions = sWorld.GetAllSessions();
    for (const auto& entry : sessions)
    {
        auto session = entry.second;
        if (!session)
        {
            continue;
        }

        auto target = session->GetPlayer();
        if (!target)
        {
            continue;
        }

        if (ignorePlayer)
        {
            if (target->GetGUID() == ignorePlayer->GetGUID())
            {
                continue;
            }
        }

        ChatHandler(session).SendSysMessage(message);
    }
}
