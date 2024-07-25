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
        return "|cffC79D6E";
        break;
    case CLASS_HUNTER:
        return "|cffC79D6E";
        break;
    case CLASS_ROGUE:
        return "|cffC79D6E";
        break;
    case CLASS_PRIEST:
        return "|cffC79D6E";
        break;
    case CLASS_SHAMAN:
        return "|cffC79D6E";
        break;
    case CLASS_MAGE:
        return "|cffC79D6E";
        break;
    case CLASS_WARLOCK:
        return "|cffC79D6E";
        break;
    case CLASS_DRUID:
        return "|cffC79D6E";
        break;
    }

    return "|cffFFFFFF";
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