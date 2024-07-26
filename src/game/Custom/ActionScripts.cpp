#include "ActionMgr.h"

#include "FlexibleInstances/FlexibleInstances.h"
#include "PlayerAnnouncer/PlayerAnnouncer.h"

void ActionMgr::Inititalize(bool reload /* = false */)
{
    if (reload)
    {
        actionScripts.clear();
        actionScriptsEnabled.clear();
        actionScriptsConfig.clear();

        // Re-fill the action scripts map with empty action types.
        for (uint32 i = 0; i < ACTION_TYPES_END; ++i)
        {
            actionScripts.emplace(i, std::vector<ActionScript*>());
        }
    }

    LoadActionScriptsEnabled();
    LoadActionScriptsConfig();

    sActionMgr.RegisterActions(new FlexibleInstancesScript(), {
        ACTION_ON_PLAYER_ENTER_MAP,
        ACTION_ON_PLAYER_EXIT_MAP,
        ACTION_ON_PLAYER_GAIN_EXP,
        ACTION_ON_CREATURE_UPDATE,
        ACTION_ON_UNIT_DAMAGE,
        ACTION_ON_UNIT_DAMAGE_PERIODIC,
        ACTION_ON_SEND_SPELL_DAMAGE_LOG,
        ACTION_ON_SEND_ATTACK_STATE_UPDATE,
        ACTION_ON_LOOT_GENERATE_MONEY,
        ACTION_ON_LOOT_PROCESSED,
        ACTION_ON_AFTER_CONFIG_LOADED,
        ACTION_ON_AFTER_ACTIONS_LOADED
    });

    sActionMgr.RegisterActions(new PlayerAnnouncerScript(), {
        ACTION_ON_PLAYER_LOG_IN,
        ACTION_ON_PLAYER_LOG_OUT
    });
}