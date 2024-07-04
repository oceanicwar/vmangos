#include "FlexibleInstances.h"

#include "InstanceData.h"

void FlexibleInstancesScript::OnInitializeActionScript()
{
    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Loading Flexible Instance Templates..");

    uint32 count = 0;
    auto result = WorldDatabase.PQuery("SELECT map_id, player_count, hp_multi, dmg_multi FROM flex_instance_template");

    if (result)
    {
        do
        {
            auto fields = result->Fetch();

            FlexibleInstance flexInstance;
            flexInstance.MapId = fields[0].GetUInt32();
            flexInstance.PlayerCount = fields[1].GetUInt32();
            flexInstance.HealthMultiplier = fields[2].GetFloat();
            flexInstance.DamageMultiplier = fields[3].GetFloat();

            auto it = flexibleInstanceTemplates.find(flexInstance.MapId);
            if (it == flexibleInstanceTemplates.end())
            {
                std::unordered_map<uint32, FlexibleInstance> instanceMap;
                flexibleInstanceTemplates.emplace(flexInstance.MapId, instanceMap);
            }

            it = flexibleInstanceTemplates.find(flexInstance.MapId);
            it->second.emplace(flexInstance.PlayerCount, flexInstance);

            count++;
        } while (result->NextRow());
    }

    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, ">> Loaded '%u' Flexible Instance Templates.", count);
}

void FlexibleInstancesScript::OnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap)
{
    if (!player || !newMap)
    {
        return;
    }

    if (!newMap->IsDungeon() && !newMap->IsRaid())
    {
        return;
    }
    
    AddPlayerToInstance(newMap->GetInstanceId(), player);
    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Player '%s' entered instance map '%u'.", player->GetName(), newMap->GetId());
}

void FlexibleInstancesScript::OnPlayerExitMap(Player* player, Map* map)
{
    if (!player || !map)
    {
        return;
    }

    if (!map->IsDungeon() && !map->IsRaid())
    {
        return;
    }

    RemovePlayerFromInstance(map->GetInstanceId(), player);
    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Player '%s' left instance map '%u'.", player->GetName(), map->GetId());
}

void FlexibleInstancesScript::OnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff)
{
    if (!creature)
    {
        return;
    }

    auto map = creature->GetMap();
    if (!map)
    {
        return;
    }

    if (!IsFlexibleInstance(map))
    {
        return;
    }

    // Do not scale creatures in combat.
    if (creature->IsInCombat())
    {
        return;
    }

    auto instanceData = map->GetInstanceData();
    if (!instanceData)
    {
        return;
    }

    // Do not scale during an instance encounter.
    if (instanceData->IsEncounterInProgress())
    {
        return;
    }

    uint32 mapId = map->GetId();
    uint32 instanceId = map->GetInstanceId();

    uint32 playerCount = GetPlayerCountForInstance(instanceId);

    // There are no players in the instance.
    if (!playerCount)
    {
        return;
    }

    auto multipliers = GetMultipliersForPlayerCount(mapId, playerCount);

    if (!multipliers)
    {
        // No template was found, leave at last scaling value.
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "No flexible instance template was suitable for player count '%u' and map '%u'.", playerCount, mapId);
        return;
    }

    auto creatureInfo = creature->GetCreatureInfo();
    uint32 rank = creature->IsPet() ? 0 : creatureInfo->rank;

    auto creatureCLS = creature->GetClassLevelStats();

    float const healthMod = creature->_GetHealthMod(rank);
    uint32 const health = std::max(1u, uint32(roundf(healthMod * creatureCLS->health * creatureInfo->health_multiplier * multipliers->HealthMultiplier)));

    creature->SetMaxHealth(health);
}

const FlexibleInstance* FlexibleInstancesScript::GetMultipliersForPlayerCount(uint32 mapId, uint32 playerCount)
{
    // Find templates for map.
    auto it = flexibleInstanceTemplates.find(mapId);
    if (it == flexibleInstanceTemplates.end())
    {
        return nullptr;
    }

    // Find the closest template for player count.
    const FlexibleInstance* selectedTemplate = nullptr;

    for (auto const& it2 : it->second)
    {
        if (playerCount <= it2.second.PlayerCount)
        {
            if (!selectedTemplate)
            {
                selectedTemplate = &it2.second;
                continue;
            }

            if (it2.second.PlayerCount < selectedTemplate->PlayerCount)
            {
                selectedTemplate = &it2.second;
            }
        }
    }

    return selectedTemplate;
}

bool FlexibleInstancesScript::IsFlexibleInstance(Map* map)
{
    if (!map->IsDungeon() && !map->IsRaid())
    {
        return false;
    }

    if (flexibleInstanceTemplates.find(map->GetId()) == flexibleInstanceTemplates.end())
    {
        return false;
    }

    return true;
}

void FlexibleInstancesScript::AddPlayerToInstance(uint32 instanceId, Player* player)
{
    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        std::unordered_set<uint64> players;
        players.emplace(player->GetGUID());

        flexibleInstances.emplace(instanceId, players);

        return;
    }

    auto it2 = it->second.find(player->GetGUID());
    if (it2 != it->second.end())
    {
        // Player already exists in instance.
        return;
    }

    it->second.emplace(player->GetGUID());
}

void FlexibleInstancesScript::RemovePlayerFromInstance(uint32 instanceId, Player* player)
{
    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        // Instance does not exist, no need to remove.
        return;
    }

    auto it2 = it->second.find(player->GetGUID());
    if (it2 == it->second.end())
    {
        // Player is not inside the instance.
        return;
    }

    it->second.erase(it2);
}

uint32 FlexibleInstancesScript::GetPlayerCountForInstance(uint32 instanceId)
{
    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        // Instance does not exist.
        return 0;
    }

    return it->second.size();
}