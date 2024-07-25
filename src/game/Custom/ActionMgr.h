#ifndef _ACTIONMGR_H
#define _ACTIONMGR_H

#include "Item.h"
#include "Player.h"
#include "Policies/Singleton.h"
#include "Spell.h"

// Used by the OnPlayerGainExperience hook to determine XP source.
enum XPSource {
    XP_SOURCE_EXPLORATION = 0,
    XP_SOURCE_QUEST = 1,
    XP_SOURCE_KILL = 2
};

class ActionScript
{
public:
    ActionScript(const char* name) 
    {
        _name = name;
    }

    /* ==================== SCRIPT HOOKS ==================== */

    // Triggered when the world settings are initially loaded.
    virtual void OnInitializeActionScript() { }

    /* ====================================================== */

    /* ==================== PLAYER HOOKS ==================== */

    // Triggered when a player uses an item.
    virtual void OnPlayerUseItem(Player* player, Item* item) { }

    // Triggered when a player casts a spell.
    virtual void OnPlayerCastSpell(Player* player, Spell* spell) { }

    // Triggered when the core checks if a spell fits the class and race for a player.
    virtual bool OnPlayerIsSpellFitByClassAndRace(const Player* player, uint32 spellId) { return false; }

    // Triggered when the core sends spell damage logs to a player.
    virtual uint32 OnSendSpellDamageLog(SpellNonMeleeDamage const* log) { return 0; }

    // Triggered when the core sends melee damage logs to a player.
    virtual uint32 OnSendAttackStateUpdate(CalcDamageInfo const* log) { return 0; }

    // Triggered when a player logs in.
    virtual void OnPlayerLogin(Player* player) { }

    // Triggered when a player logs out.
    virtual void OnPlayerLogout(Player* player) { }

    // Triggered when a player enters a map.
    virtual void OnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap) { }

    // Triggered when a player exits a map.
    virtual void OnPlayerExitMap(Player* player, Map* map) { }

    // Triggered when a player gains experience from various sources.
    virtual void OnPlayerGainExperience(Player* player, uint32& exp, XPSource source) { }

    /* ====================================================== */

    /* ===================== UNIT HOOKS ===================== */

    // Triggered after the calculations for damage have passed when a unit deals damage to another unit.
    virtual void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) { }

    // Triggered after the calculations for damage have passed when a unit deals periodic damage to another unit.
    virtual void OnUnitDamagePeriodic(Unit* aggressor, Unit* victim, uint32& damage, Aura* aura) { }

    /* ====================================================== */

    /* =================== CREATURE HOOKS =================== */

    // Triggered before the creature updates.
    virtual void OnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff) { }

    /* ====================================================== */

    /* ===================== LOOT HOOKS ===================== */

    // Triggered when money is generated for loot.
    virtual void OnGenerateLootMoney(Loot* loot, uint32& money) { }

    // Triggered after loot has been processed/generated.
    virtual void OnLootProcessed(Loot* loot) { }

    /* ====================================================== */

    /* ===================== MISC HOOKS ===================== */

    virtual void OnAfterConfigLoaded(bool reload) { }

    /* ====================================================== */

    const char* GetName()
    {
        return _name;
    }

private:
    const char* _name;
};

enum ActionTypes : uint32
{
    ACTION_ON_PLAYER_USE_ITEM = 0,
    ACTION_ON_PLAYER_CAST_SPELL = 1,
    ACTION_ON_PLAYER_SPELL_FIT_CLASS_RACE = 2,

    ACTION_ON_PLAYER_LOG_IN = 3,
    ACTION_ON_PLAYER_LOG_OUT = 4,

    ACTION_ON_PLAYER_ENTER_MAP = 5,
    ACTION_ON_PLAYER_EXIT_MAP = 6,

    ACTION_ON_PLAYER_GAIN_EXP = 7,

    ACTION_ON_UNIT_DAMAGE = 8,
    ACTION_ON_UNIT_DAMAGE_PERIODIC = 9,

    ACTION_ON_CREATURE_UPDATE = 10,

    ACTION_ON_SEND_SPELL_DAMAGE_LOG = 11,
    ACTION_ON_SEND_ATTACK_STATE_UPDATE = 12,

    ACTION_ON_LOOT_GENERATE_MONEY = 13,
    ACTION_ON_LOOT_PROCESSED = 14,

    ACTION_ON_ACTIONSCRIPT_INITIALIZE = 15,

    ACTION_ON_AFTER_CONFIG_LOADED = 16,

    ACTION_TYPES_END = 17
};

class ActionMgr
{
public:
    ActionMgr();
    ~ActionMgr();

    void Inititalize();
    void RegisterActions(ActionScript* script, std::vector<uint32> actions);

    void ActionOnPlayerUseItem(Player* player, Item* item);
    void ActionOnPlayerCastSpell(Player* player, Spell* spell);
    bool ActionOnPlayerIsSpellFitByClassAndRace(const Player* player, uint32 spellId);

    void ActionOnPlayerLogin(Player* player);
    void ActionOnPlayerLogout(Player* player);

    void ActionOnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap);
    void ActionOnPlayerExitMap(Player* player, Map* map);

    void ActionOnPlayerGainExperience(Player* player, uint32& exp, XPSource source);

    void ActionOnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected);
    void ActionOnUnitDamagePeriodic(Unit* aggressor, Unit* victim, uint32& damage, Aura* aura);

    void ActionOnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff);

    void ActionOnInitializeActionScript();

    uint32 ActionOnSendSpellDamageLog(SpellNonMeleeDamage const* log);
    uint32 ActionOnSendAttackStateUpdate(CalcDamageInfo const* log);

    void ActionOnGenerateLootMoney(Loot* loot, uint32& money);
    void ActionOnLootProcessed(Loot* loot);

    void ActionOnAfterConfigLoaded(bool reload);

private:
    std::map<uint32, std::vector<ActionScript*>> actionScripts;
};

#define sActionMgr MaNGOS::Singleton<ActionMgr>::Instance()

#endif