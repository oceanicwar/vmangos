#ifndef _PLAYER_ACTION_SCRIPT_H
#define _PLAYER_ACTION_SCRIPT_H

#include "ActionMgr.h"

class PlayerActionScript : public ActionScript
{
public:
    PlayerActionScript() : ActionScript("PlayerActionScript") { }

public:
    void OnPlayerUseItem(Player* player, Item* item) override
    {
        WorldPacket serverMsg(SMSG_SERVER_MESSAGE);
        serverMsg << 3; // ServerMessageType::Custom
        serverMsg << "OnPlayerUseItem";

        player->SendDirectMessage(&serverMsg);
    }
};

#endif