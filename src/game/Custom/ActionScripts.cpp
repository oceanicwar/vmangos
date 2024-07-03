#include "ActionMgr.h"

#include "ClassicPlus/ClassicPlusScripts.h"

// Register your action scripts using this function.
void ActionMgr::Inititalize()
{
    sActionMgr.RegisterActions(new ClassicPlusScripts(), {
        ACTION_ON_PLAYER_ENTER_MAP,
        ACTION_ON_CREATURE_UPDATE,
        ACTION_ON_ACTIONSCRIPT_INITIALIZE
    });
}