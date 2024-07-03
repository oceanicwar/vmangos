#include "ActionMgr.h"

#include "ClassicPlus/ClassicPlusScripts.h"

// Register your action scripts using this function.
void ActionMgr::Inititalize()
{
    sActionMgr.RegisterActions(new ClassicPlusScripts(), {
        ACTION_ON_UNIT_DAMAGE,
        ACTION_ON_SEND_SPELL_DAMAGE_LOG
    });
}