#ifndef _CUSTOM_FLEXIBLE_INSTANCES_H
#define _CUSTOM_FLEXIBLE_INSTANCES_H

#include "ActionMgr.h"

enum FlexibleInstancesConstants {
};

struct FlexibleInstanceTemplate {
    uint32 MapId;
    uint32 PlayerCount;

    float HealthMultiplier;
    float DamageMultiplier;
    float ExpMultiplier;
    float GoldMultiplier;
    float ItemMultiplier;
};

struct FlexibleInstance {
    FlexibleInstanceTemplate const* Template;

    std::unordered_set<uint64> Players;
    bool NotifiedPlayers;
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
    uint32 OnSendSpellDamageLog(SpellNonMeleeDamage const* log) override;
    uint32 OnSendAttackStateUpdate(CalcDamageInfo const* log) override;
    void OnPlayerGainExperience(Player* player, uint32& xp, XPSource source) override;
    void OnGenerateLootMoney(Loot* loot, uint32& money) override;
    void OnLootProcessed(Loot* loot) override;

    bool IsFlexibleInstance(Map* map);
    void AddPlayerToInstance(uint32 instanceId, Player* player);
    void RemovePlayerFromInstance(uint32 instanceId, Player* player);
    uint32 GetPlayerCountForInstance(uint32 instanceId);
    const FlexibleInstanceTemplate* GetMultipliersForPlayerCount(uint32 mapId, uint32 playerCount);

    void UpdateFlexibility(Map* map);
    void NotifyFlexibilityChanged(Map* map, Player* skipPlayer = nullptr);
    FlexibleInstance* GetFlexibleInstance(Map* map);

private:
    std::unordered_map<uint32 /* MapId */, std::unordered_map<uint32 /* PlayerCount */, FlexibleInstanceTemplate>> flexibleInstanceTemplates;
    std::unordered_map<uint32 /* InstanceId */, FlexibleInstance> flexibleInstances;
};

#endif