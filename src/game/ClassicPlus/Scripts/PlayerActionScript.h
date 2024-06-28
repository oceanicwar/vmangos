#ifndef _PLAYER_ACTION_SCRIPT_H
#define _PLAYER_ACTION_SCRIPT_H

#include "ActionMgr.h"

class PlayerActionScript : public ActionScript
{
public:
    PlayerActionScript() : ActionScript("PlayerActionScript") { }

public:
    void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) override
    {
        if(!spell || !spellProto)
        {
            return;
        }

        if(!spellProto->Id != 2537 /* Crusader Strike - Rank 1 */)
        {
            return;
        }

        Player* player = aggressor->ToPlayer();
        if(!player)
        {
            return;
        }

        // Double the damage of Crusader Strike - Rank 1, maybe scale this with weapon damage later?
        damage = damage * 2;
    }
};

#endif