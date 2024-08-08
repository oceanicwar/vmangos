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

std::string ActionScriptHelper::GetSecurityName(AccountTypes accountType)
{
    switch (accountType)
    {
    case AccountTypes::SEC_PLAYER:
        return "Player";

    case AccountTypes::SEC_MODERATOR:
        return "Moderator";

    case AccountTypes::SEC_TICKETMASTER:
        return "Ticket Master";

    case AccountTypes::SEC_GAMEMASTER:
        return "Game Master";

    case AccountTypes::SEC_BASIC_ADMIN:
        return "Basic Administrator";

    case AccountTypes::SEC_DEVELOPER:
        return "Developer";

    case AccountTypes::SEC_ADMINISTRATOR:
        return "Administrator";

    case AccountTypes::SEC_CONSOLE:
        return "Console";
    }
}

bool ActionScriptHelper::ReplaceString(std::string& s, std::string target, std::string value)
{
    auto it = s.find(target);
    if (it == std::string::npos)
    {
        return false;
    }

    s.replace(it, target.length(), value);

    return true;
}
