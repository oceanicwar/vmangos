#ifndef _CUSTOM_FLEXIBLE_INSTANCES_H
#define _CUSTOM_FLEXIBLE_INSTANCES_H

#include "ActionMgr.h"

enum FlexibleInstancesConstants {
};

struct FlexibleInstance {
    uint32 MapId;
    uint32 PlayerCount;

    float HealthMultiplier;
    float DamageMultiplier;
};

class FlexibleInstancesScript : public ActionScript
{
public:
    FlexibleInstancesScript() : ActionScript("FlexibleInstancesScript") { }

public:
    void OnInitializeActionScript() override;
    void OnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap) override;
    void OnPlayerExitMap(Player* player, Map* map) override;
    void OnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff) override;
    void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) override;
    uint32 OnSendSpellDamageLog(SpellNonMeleeDamage const* log) override;
    uint32 OnSendAttackStateUpdate(CalcDamageInfo const* log) override;

    bool IsFlexibleInstance(Map* map);
    void AddPlayerToInstance(uint32 instanceId, Player* player);
    void RemovePlayerFromInstance(uint32 instanceId, Player* player);
    uint32 GetPlayerCountForInstance(uint32 instanceId);
    const FlexibleInstance* GetMultipliersForPlayerCount(uint32 mapId, uint32 playerCount);

private:
    std::unordered_map<uint32 /* MapId */, std::unordered_map<uint32 /* PlayerCount */, FlexibleInstance>> flexibleInstanceTemplates;
    std::unordered_map<uint32 /* InstanceId */, std::unordered_set<uint64 /* Player GUID */>> flexibleInstances;
};

#endif