#include "ClassicPlusScripts.h"

void ClassicPlusScripts::OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected)
{
    if (!spell || !spellProto)
    {
        return;
    }

    Player* player = aggressor->ToPlayer();
    if (!player)
    {
        return;
    }

    uint32 crusaderStrike = 2537; /* Crusader Strike - Rank 1 */
    if (spellProto->Id != crusaderStrike)
    {
        return;
    }

    // Double the damage of Crusader Strike - Rank 1, maybe scale this with weapon damage later?
    uint32 newDamage = damage * 2;
    damage = newDamage;
}