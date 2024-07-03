#ifndef _CUSTOM_CLASSIC_PLUS_H
#define _CUSTOM_CLASSIC_PLUS_H

#include "ActionMgr.h"

class ClassicPlusScripts : public ActionScript
{
public:
    ClassicPlusScripts() : ActionScript("ClassicPlusScripts") { }

public:
    void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) override;
};

#endif