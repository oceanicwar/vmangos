#ifndef _CUSTOM_PLAYER_ANNOUNCER_H
#define _CUSTOM_PLAYER_ANNOUNCER_H

#include "ActionMgr.h"

class PlayerAnnouncerScript : public ActionScript
{
public:
    PlayerAnnouncerScript() : ActionScript("PlayerAnnouncer") { }

public:
    void OnPlayerLogin(Player* player, bool firstLogin) override;
    void OnPlayerLogout(Player* player) override;
};

#endif
