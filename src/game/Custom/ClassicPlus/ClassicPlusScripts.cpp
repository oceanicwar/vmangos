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

    if (IsCrusaderStrike(spellProto->Id))
    {
        damage = GetCrusaderStrikeDamage(player, damage);
    }
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

    if (IsCrusaderStrike(log->SpellID))
    {
        return GetCrusaderStrikeDamage(player, log->damage);
    }

    return 0;
}

bool ClassicPlusScripts::IsCrusaderStrike(uint32 spellId)
{
    return spellId == 2537 ||   /* Rank 1 */
           spellId == 8823 ||   /* Rank 2 */
           spellId == 8824 ||   /* Rank 3 */
           spellId == 10336 ||  /* Rank 4 */
           spellId == 10337;    /* Rank 5 */
}

uint32 ClassicPlusScripts::GetCrusaderStrikeDamage(Player* player, uint32 originalDamage)
{
    float minDmg = player->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
    float maxDmg = player->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE);

    return originalDamage + urand(minDmg, maxDmg);
}