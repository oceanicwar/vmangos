#include "ActionMgr.h"

#include "PlayerActionScript.h"

// Register your action scripts using this function.
void ActionMgr::Inititalize()
{
    sActionMgr.RegisterActions(new PlayerActionScript(), {
        ACTION_ON_PLAYER_USE_ITEM,
        ACTION_ON_PLAYER_CAST_SPELL
    });
}