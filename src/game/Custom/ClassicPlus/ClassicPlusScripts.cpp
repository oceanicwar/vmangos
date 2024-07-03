#include "ClassicPlusScripts.h"

void ClassicPlusScripts::OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected)
{
    return;
}

uint32 ClassicPlusScripts::OnSendSpellDamageLog(SpellNonMeleeDamage const* log)
{
    return 0;
}