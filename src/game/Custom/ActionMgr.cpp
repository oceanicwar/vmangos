#include "ActionMgr.h"

#include "Log.h"
#include "Policies/SingletonImp.h"

INSTANTIATE_SINGLETON_1(ActionMgr);

ActionMgr::ActionMgr()
{
    // Fill the action scripts map with empty action types.
    for(uint32 i = 0; i < ACTION_TYPES_END; ++i)
    {
        actionScripts.emplace(i, std::vector<ActionScript*>());
    }
}

ActionMgr::~ActionMgr() 
{ 
    for(const auto& actions : actionScripts)
    {
        for(const auto& script : actions.second)
        {
            delete script;
        }
    }

    actionScripts.clear();
}

void ActionMgr::LoadActionScriptsEnabled()
{
    uint32 count = 0;
    auto result = WorldDatabase.PQuery("SELECT * FROM actions_enabled");

    if (result)
    {
        do
        {
            auto fields = result->Fetch();

            std::string scriptName = fields[0].GetString();
            bool enabled = fields[1].GetBool();

            actionScriptsEnabled.emplace(scriptName, enabled);

            count++;
        } while (result->NextRow());
    }
}

void ActionMgr::LoadActionScriptsConfig()
{
    uint32 count = 0;
    auto result = WorldDatabase.PQuery("SELECT * FROM actions_config");

    if (result)
    {
        do
        {
            auto fields = result->Fetch();

            std::string scriptName = fields[0].GetString();
            std::string key = fields[1].GetString();
            std::string value = fields[2].GetString();

            auto it = actionScriptsConfig.find(scriptName);
            if (it == actionScriptsConfig.end())
            {
                std::unordered_map<std::string, std::string> configMap;
                auto result = actionScriptsConfig.emplace(scriptName, configMap);

                if (!result.second)
                {
                    continue;
                }

                it = result.first;
            }

            it->second.emplace(key, value);

            count++;
        } while (result->NextRow());
    }
}

bool ActionMgr::IsActionScriptEnabled(std::string scriptName)
{
    auto it = actionScriptsEnabled.find(scriptName);
    if (it == actionScriptsEnabled.end())
    {
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "No entry was found for '%s' in actions_enabled. Disabling script.", scriptName);
        return false;
    }

    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, it->second ? "Action Script '%s' enabled." : "Action Script '%s' disabled.", scriptName);

    return it->second;
}

void ActionMgr::RegisterActions(ActionScript* script, std::vector<uint32> actions)
{
    if (!IsActionScriptEnabled(script->GetName()))
    {
        delete script;
        return;
    }

    for(auto it = actions.begin(); it != actions.end(); ++it)
    {
        uint32 actionType = (*it);

        auto action = actionScripts.find(actionType);
        if(action == actionScripts.end())
        {
            sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Unknown action type '%u' for action script '%s'.", actionType, script->GetName());
            continue;
        }

        action->second.push_back(script);
    }

    auto it = actionScriptsConfig.find(script->GetName());
    if (it != actionScriptsConfig.end())
    {
        ActionScriptConfig config(it->second);
        script->SetConfig(config);
    }
}

void ActionMgr::ActionOnPlayerUseItem(Player* player, Item* item)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_USE_ITEM);
    if(it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if(scripts.empty())
    {
        return;
    }

    for(auto& script : scripts)
    {
        script->OnPlayerUseItem(player, item);
    }
}

void ActionMgr::ActionOnPlayerCastSpell(Player* player, Spell* spell)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_CAST_SPELL);
    if(it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if(scripts.empty())
    {
        return;
    }

    for(auto& script : scripts)
    {
        script->OnPlayerCastSpell(player, spell);
    }
}

bool ActionMgr::ActionOnPlayerIsSpellFitByClassAndRace(const Player* player, uint32 spellId)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_SPELL_FIT_CLASS_RACE);
    if(it == actionScripts.end())
    {
        return false;
    }

    auto scripts = it->second;
    if(scripts.empty())
    {
        return false;
    }

    bool result = false;

    for(auto& script : scripts)
    {
        if(script->OnPlayerIsSpellFitByClassAndRace(player, spellId))
        {
            result = true;
        }
    }

    return result;
}

void ActionMgr::ActionOnPlayerLogin(Player* player, bool firstLogin)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_LOG_IN);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnPlayerLogin(player, firstLogin);
    }
}

void ActionMgr::ActionOnPlayerLogout(Player* player)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_LOG_OUT);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnPlayerLogout(player);
    }
}

void ActionMgr::ActionOnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_ENTER_MAP);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnPlayerEnterMap(player, oldMap, newMap);
    }
}

void ActionMgr::ActionOnPlayerExitMap(Player* player, Map* map)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_EXIT_MAP);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnPlayerExitMap(player, map);
    }
}

void ActionMgr::ActionOnPlayerGainExperience(Player* player, uint32& exp, XPSource source)
{
    auto it = actionScripts.find(ACTION_ON_PLAYER_GAIN_EXP);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnPlayerGainExperience(player, exp, source);
    }
}

void ActionMgr::ActionOnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected)
{
    auto it = actionScripts.find(ACTION_ON_UNIT_DAMAGE);
    if(it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if(scripts.empty())
    {
        return;
    }

    for(auto& script : scripts)
    {
        script->OnUnitDamage(aggressor, victim, damage, cleanDamage, damagetype, damageSchoolMask, spellProto, durabilityLoss, spell, reflected);
    }
}

void ActionMgr::ActionOnUnitDamagePeriodic(Unit* aggressor, Unit* victim, uint32& damage, Aura* aura)
{
    auto it = actionScripts.find(ACTION_ON_UNIT_DAMAGE_PERIODIC);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnUnitDamagePeriodic(aggressor, victim, damage, aura);
    }
}

uint32 ActionMgr::ActionOnSendSpellDamageLog(SpellNonMeleeDamage const* log)
{
    auto it = actionScripts.find(ACTION_ON_SEND_SPELL_DAMAGE_LOG);
    if (it == actionScripts.end())
    {
        return 0;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return 0;
    }

    uint32 newDamage = 0;
    for (auto& script : scripts)
    {
        auto damage = script->OnSendSpellDamageLog(log);
        if (damage)
        {
            newDamage = damage;
        }
    }

    return newDamage;
}

uint32 ActionMgr::ActionOnSendAttackStateUpdate(CalcDamageInfo const* log)
{
    auto it = actionScripts.find(ACTION_ON_SEND_SPELL_DAMAGE_LOG);
    if (it == actionScripts.end())
    {
        return 0;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return 0;
    }

    uint32 newDamage = 0;
    for (auto& script : scripts)
    {
        auto damage = script->OnSendAttackStateUpdate(log);
        if (damage)
        {
            newDamage = damage;
        }
    }

    return newDamage;
}

void ActionMgr::ActionOnGenerateLootMoney(Loot* loot, uint32& money)
{
    auto it = actionScripts.find(ACTION_ON_LOOT_GENERATE_MONEY);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnGenerateLootMoney(loot, money);
    }
}

void ActionMgr::ActionOnLootProcessed(Loot* loot)
{
    auto it = actionScripts.find(ACTION_ON_LOOT_PROCESSED);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnLootProcessed(loot);
    }
}

void ActionMgr::ActionOnAfterConfigLoaded(bool reload /* = false */)
{
    auto it = actionScripts.find(ACTION_ON_AFTER_CONFIG_LOADED);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnAfterConfigLoaded(reload);
    }
}

void ActionMgr::ActionOnAfterActionsLoaded(bool reload /* = false */)
{
    auto it = actionScripts.find(ACTION_ON_AFTER_ACTIONS_LOADED);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnAfterActionsLoaded(reload);
    }
}

void ActionMgr::ActionOnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff)
{
    auto it = actionScripts.find(ACTION_ON_CREATURE_UPDATE);
    if (it == actionScripts.end())
    {
        return;
    }

    auto scripts = it->second;
    if (scripts.empty())
    {
        return;
    }

    for (auto& script : scripts)
    {
        script->OnCreatureUpdate(creature, update_diff, diff);
    }
}
