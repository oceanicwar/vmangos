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

uint32 ClassicPlusScripts::OnSendSpellDamageLog(SpellNonMeleeDamage const* log)
{
    if (!log)
    {
        return 0;
    }

    if (!log->attacker || !log->attacker->IsPlayer())
    {
        return 0;
    }

    Player* player = log->attacker->ToPlayer();
    if (!player)
    {
        return 0;
    }

    uint32 crusaderStrike = 2537; /* Crusader Strike - Rank 1 */
    if (log->SpellID != crusaderStrike)
    {
        return 0;
    }

    // Double the damage of Crusader Strike - Rank 1, maybe scale this with weapon damage later?
    uint32 newDamage = log->damage * 2;
    return newDamage;
}