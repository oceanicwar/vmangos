#include "ActionMgr.h"

#include "FlexibleInstances/FlexibleInstances.h"

// Register your action scripts using this function.
void ActionMgr::Inititalize()
{
    sActionMgr.RegisterActions(new FlexibleInstancesScript(), {
        ACTION_ON_PLAYER_ENTER_MAP,
        ACTION_ON_PLAYER_EXIT_MAP,
        ACTION_ON_CREATURE_UPDATE,
        ACTION_ON_ACTIONSCRIPT_INITIALIZE
    });
}