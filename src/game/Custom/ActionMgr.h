#ifndef _ACTIONMGR_H
#define _ACTIONMGR_H

#include "Item.h"
#include "Player.h"
#include "Policies/Singleton.h"
#include "Spell.h"

class ActionScript
{
public:
    ActionScript(const char* name) 
    {
        _name = name;
    }

    /* ==================== PLAYER HOOKS ==================== */

    // Triggered when a player uses an item.
    virtual void OnPlayerUseItem(Player* player, Item* item) { }

    // Triggered when a player casts a spell.
    virtual void OnPlayerCastSpell(Player* player, Spell* spell) { }

    // Triggered when the core checks if a spell fits the class and race for a player.
    virtual bool OnPlayerIsSpellFitByClassAndRace(const Player* player, uint32 spellId) { return false; }

    // Triggered when the core sends damage logs to a player.
    virtual uint32 OnSendSpellDamageLog(SpellNonMeleeDamage const* log) { return 0; }

    /* ====================================================== */

    /* ===================== UNIT HOOKS ===================== */

    // Triggered after the calculations for damage have passed when a unit deals damage to another unit.
    virtual void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) { }

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

    ACTION_ON_UNIT_DAMAGE = 3,
    ACTION_ON_SEND_SPELL_DAMAGE_LOG = 4,

    ACTION_TYPES_END = 5
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

    void ActionOnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected);
    uint32 ActionOnSendSpellDamageLog(SpellNonMeleeDamage const* log);

private:
    std::map<uint32, std::vector<ActionScript*>> actionScripts;
};

#define sActionMgr MaNGOS::Singleton<ActionMgr>::Instance()

#endif