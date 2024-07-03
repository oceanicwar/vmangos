#include "ActionMgr.h"

#include "ClassicPlus/ClassicPlusScripts.h"

// Register your action scripts using this function.
void ActionMgr::Inititalize()
{
    sActionMgr.RegisterActions(new ClassicPlusScripts(), {
        ACTION_ON_UNIT_DAMAGE
    });
}