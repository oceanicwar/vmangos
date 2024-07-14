#ifndef _CUSTOM_FLEXIBLE_INSTANCES_H
#define _CUSTOM_FLEXIBLE_INSTANCES_H

#include "ActionMgr.h"

#define MetadataFlexibleInstances "FlexibleInstances"

enum FlexibleInstancesConstants {
    SPELL_ENTRY_FLEXIBLE = 50001
};

struct FlexibleInstanceTemplate {
    uint32 PlayerCount;

    float HealthMultiplier;
    float DamageMultiplier;
    float ExpMultiplier;
    float GoldMultiplier;
    float ItemMultiplier;
};

class FlexibleInstancesScript : public ActionScript
{
public:
    FlexibleInstancesScript() : ActionScript("FlexibleInstancesScript") { }

public:
    void OnAfterConfigLoaded(bool reload) override;

    void OnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap) override;
    void OnPlayerExitMap(Player* player, Map* map) override;
    void OnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff) override;
    void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) override;
    void OnUnitDamagePeriodic(Unit* aggressor, Unit* victim, uint32& damage, Aura* aura) override;
    uint32 OnSendSpellDamageLog(SpellNonMeleeDamage const* log) override;
    uint32 OnSendAttackStateUpdate(CalcDamageInfo const* log) override;
    void OnPlayerGainExperience(Player* player, uint32& xp, XPSource source) override;
    void OnGenerateLootMoney(Loot* loot, uint32& money) override;
    void OnLootProcessed(Loot* loot) override;

    bool IsFlexibleInstance(Map* map);
    uint32 GetPlayerCountForMap(Map* map);
    const FlexibleInstanceTemplate* GetTemplateForPlayerCount(uint32 mapId, uint32 playerCount);
    bool CheckTemplatesMatch(const FlexibleInstanceTemplate* template1, const FlexibleInstanceTemplate* template2);

    void UpdateFlexTemplateForMap(Map* map);
    void UpdateFlexTemplateForCreature(Creature* creature);
    void NotifyFlexibilityChanged(Map* map, Player* skipPlayer = nullptr);

private:
    std::unordered_map<uint32 /* MapId */, std::unordered_map<uint32 /* PlayerCount */, FlexibleInstanceTemplate>> flexibleInstanceTemplates;
};

#endif