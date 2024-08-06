#include "AutoGuild.h"

#include "Guild.h"

void AutoGuildScript::OnPlayerLogin(Player* player, bool firstLogin)
{
    if (!player || !firstLogin)
    {
        return;
    }

    if (player->GetGuildId())
    {
        return;
    }

    uint32 guildId = this->GetConfig()->GetValue<uint32>("GuildId");

    auto guild = sGuildMgr.GetGuildById(guildId);
    if (!guild)
    {
        return;
    }

    if (guild->AddMember(player->GetObjectGuid(), guild->GetLowestRank()) != GuildAddStatus::OK)
    {
        return;
    }

    guild->LogGuildEvent(GUILD_EVENT_LOG_JOIN_GUILD, player->GetObjectGuid());
    guild->BroadcastEvent(GE_JOINED, player->GetObjectGuid(), player->GetName());
}
