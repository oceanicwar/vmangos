#ifndef _CUSTOM_AUTO_GUILD_H
#define _CUSTOM_AUTO_GUILD_H

#include "ActionMgr.h"

class AutoGuildScript : public ActionScript
{
public:
    AutoGuildScript() : ActionScript("AutoGuild") { }

public:
    void OnPlayerLogin(Player* player, bool firstLogin) override;
};

#endif
