#include "FlexibleInstances.h"

#include "Chat.h"
#include "InstanceData.h"

void FlexibleInstancesScript::OnInitializeActionScript()
{
    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Loading Flexible Instance Templates..");

    uint32 count = 0;
    auto result = WorldDatabase.PQuery("SELECT map_id, player_count, hp_multi, dmg_multi, xp_multi, gold_multi, item_multi FROM flex_instance_template");

    if (result)
    {
        do
        {
            auto fields = result->Fetch();

            FlexibleInstanceTemplate flexTemplate;
            flexTemplate.MapId = fields[0].GetUInt32();
            flexTemplate.PlayerCount = fields[1].GetUInt32();
            flexTemplate.HealthMultiplier = fields[2].GetFloat();
            flexTemplate.DamageMultiplier = fields[3].GetFloat();
            flexTemplate.ExpMultiplier = fields[4].GetFloat();
            flexTemplate.GoldMultiplier = fields[5].GetFloat();
            flexTemplate.ItemMultiplier = fields[6].GetFloat();

            auto it = flexibleInstanceTemplates.find(flexTemplate.MapId);
            if (it == flexibleInstanceTemplates.end())
            {
                std::unordered_map<uint32, FlexibleInstanceTemplate> instanceMap;
                flexibleInstanceTemplates.emplace(flexTemplate.MapId, instanceMap);
            }

            it = flexibleInstanceTemplates.find(flexTemplate.MapId);
            it->second.emplace(flexTemplate.PlayerCount, flexTemplate);

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

    if (!IsFlexibleInstance(newMap))
    {
        return;
    }
    
    AddPlayerToInstance(newMap->GetInstanceId(), player);
    UpdateFlexibility(newMap);
    NotifyFlexibilityChanged(newMap);

    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Player '%s' entered instance map '%u'.", player->GetName(), newMap->GetId());
}

void FlexibleInstancesScript::OnPlayerExitMap(Player* player, Map* map)
{
    if (!player || !map)
    {
        return;
    }

    if (!IsFlexibleInstance(map))
    {
        return;
    }

    RemovePlayerFromInstance(map->GetInstanceId(), player);
    UpdateFlexibility(map);
    NotifyFlexibilityChanged(map, player);

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

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return;
    }

    auto creatureInfo = creature->GetCreatureInfo();
    uint32 rank = creature->IsPet() ? 0 : creatureInfo->rank;

    auto creatureCLS = creature->GetClassLevelStats();

    float const healthMod = creature->_GetHealthMod(rank);
    uint32 const health = std::max(1u, uint32(roundf(healthMod * creatureCLS->health * creatureInfo->health_multiplier * flexInstance->Template->HealthMultiplier)));

    if (creature->GetMaxHealth() == health)
    {
        return;
    }

    creature->SetMaxHealth(health);
}

void FlexibleInstancesScript::OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected)
{
    if (!aggressor || !victim)
    {
        return;
    }

    if (aggressor->IsPlayer() || !aggressor->IsCreature())
    {
        return;
    }

    auto creature = aggressor->ToCreature();
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

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return;
    }

    uint32 newDamage = std::max(1u, uint32(roundf(damage * flexInstance->Template->DamageMultiplier)));
    damage = newDamage;
}

uint32 FlexibleInstancesScript::OnSendSpellDamageLog(SpellNonMeleeDamage const* log)
{
    auto aggressor = log->attacker;
    auto victim = log->target;
    
    if (!aggressor || !victim)
    {
        return 0;
    }

    if (aggressor->IsPlayer() || !aggressor->IsCreature())
    {
        return 0;
    }

    auto creature = aggressor->ToCreature();
    if (!creature)
    {
        return 0;
    }

    auto map = creature->GetMap();
    if (!map)
    {
        return 0;
    }

    if (!IsFlexibleInstance(map))
    {
        return 0;
    }

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return 0;
    }

    return std::max(1u, uint32(roundf(log->damage * flexInstance->Template->DamageMultiplier)));
}

uint32 FlexibleInstancesScript::OnSendAttackStateUpdate(CalcDamageInfo const* log)
{
    auto aggressor = log->attacker;
    auto victim = log->target;

    if (!aggressor || !victim)
    {
        return 0;
    }

    if (aggressor->IsPlayer() || !aggressor->IsCreature())
    {
        return 0;
    }

    auto creature = aggressor->ToCreature();
    if (!creature)
    {
        return 0;
    }

    auto map = creature->GetMap();
    if (!map)
    {
        return 0;
    }

    if (!IsFlexibleInstance(map))
    {
        return 0;
    }

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return 0;
    }

    return std::max(1u, uint32(roundf(log->totalDamage * flexInstance->Template->DamageMultiplier)));
}

void FlexibleInstancesScript::OnPlayerGainExperience(Player* player, uint32& xp, XPSource source)
{
    if (!player || xp == 0)
    {
        return;
    }

    if (source != XPSource::XP_SOURCE_KILL)
    {
        return;
    }

    auto map = player->GetMap();
    if (!map)
    {
        return;
    }

    if (!IsFlexibleInstance(map))
    {
        return;
    }

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return;
    }

    xp = xp * flexInstance->Template->ExpMultiplier;
}

void FlexibleInstancesScript::OnGenerateLootMoney(Loot* loot, uint32& money)
{
    if (!loot || money == 0)
    {
        return;
    }

    auto lootTarget = loot->GetLootTarget();
    if (!lootTarget)
    {
        return;
    }

    auto map = lootTarget->GetMap();
    if (!IsFlexibleInstance(map))
    {
        return;
    }

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return;
    }

    money = money * flexInstance->Template->GoldMultiplier;
}

void FlexibleInstancesScript::OnLootProcessed(Loot* loot)
{
    if (!loot)
    {
        return;
    }

    auto lootTarget = loot->GetLootTarget();
    if (!lootTarget)
    {
        return;
    }

    auto map = lootTarget->GetMap();
    if (!IsFlexibleInstance(map))
    {
        return;
    }

    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance || !flexInstance->Template)
    {
        return;
    }

    auto items = loot->items;
    std::vector<const LootItem*> removableItems;

    for (const auto& item : items)
    {
        auto itemProto = sObjectMgr.GetItemPrototype(item.itemid);
        if (!itemProto)
        {
            continue;
        }

        // These types of items should not be removed from loot table.
        if (itemProto->Class == ITEM_CLASS_QUEST || 
            itemProto->Class == ITEM_CLASS_KEY)
        {
            continue;
        }

        removableItems.push_back(&item);
    }

    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, ">> '%u' loot items generated, shuffling..", removableItems.size());

    // Shuffle the items to randomize removal.
    std::random_shuffle(removableItems.begin(), removableItems.end());

    auto itemMulti = flexInstance->Template->ItemMultiplier;
    auto countToRemove = static_cast<uint32>(floor(removableItems.size() - (removableItems.size() * itemMulti)));

    sLog.Out(LOG_BASIC, LOG_LVL_BASIC, ">> Removing '%u' loot items..", countToRemove);

    for (uint32 i = 0; i < countToRemove; ++i)
    {
        auto itemToRemove = removableItems[i];
        if (!itemToRemove)
        {
            continue;
        }

        auto item = std::find_if(loot->items.begin(), loot->items.end(), [itemToRemove](const LootItem& item) {
            return item.itemid == itemToRemove->itemid;
        });

        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, ">> Removing item '%u'..", item->itemid);

        loot->items.erase(item);
    }
}

const FlexibleInstanceTemplate* FlexibleInstancesScript::GetMultipliersForPlayerCount(uint32 mapId, uint32 playerCount)
{
    // Find templates for map.
    auto it = flexibleInstanceTemplates.find(mapId);
    if (it == flexibleInstanceTemplates.end())
    {
        return nullptr;
    }

    // Find the closest template for player count.
    const FlexibleInstanceTemplate* selectedTemplate = nullptr;

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

void FlexibleInstancesScript::UpdateFlexibility(Map* map)
{
    uint32 mapId = map->GetId();
    uint32 instanceId = map->GetInstanceId();
    uint32 playerCount = GetPlayerCountForInstance(instanceId);

    // There are no players in the instance.
    if (!playerCount)
    {
        return;
    }

    auto flexTemplate = GetMultipliersForPlayerCount(mapId, playerCount);
    if (!flexTemplate)
    {
        return;
    }

    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        return;
    }

    auto instanceTemplate = it->second.Template;
    if (!instanceTemplate)
    {
        it->second.Template = flexTemplate;
        it->second.NotifiedPlayers = false;
        return;
    }

    // Template already in effect.
    if (instanceTemplate->PlayerCount == flexTemplate->PlayerCount)
    {
        return;
    }

    it->second.Template = flexTemplate;
    it->second.NotifiedPlayers = false;
}

void FlexibleInstancesScript::NotifyFlexibilityChanged(Map* map, Player* skipPlayer)
{
    auto flexInstance = GetFlexibleInstance(map);
    if (!flexInstance)
    {
        return;
    }

    if (!flexInstance->Template)
    {
        return;
    }

    Map::PlayerList const& players = map->GetPlayers();
    for (const auto& playerRef : players)
    {
        auto player = playerRef.getSource();
        if (!player)
        {
            continue;
        }

        // If the player is leaving the map they probably dont need to be notified.
        if (skipPlayer && skipPlayer->GetGUID() == player->GetGUID())
        {
            continue;
        }

        ChatHandler(player->GetSession()).PSendSysMessage("|cffFFD700[Flexible Instances]: |cffFFFFFFCreature damage, health, experience, gold, and item drop rates have been adjusted for a group of |cff00FF00%u|cffFFFFFF.|r", flexInstance->Template->PlayerCount);
    }

    flexInstance->NotifiedPlayers = true;
}

FlexibleInstance* FlexibleInstancesScript::GetFlexibleInstance(Map* map)
{
    uint32 instanceId = map->GetInstanceId();

    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        return nullptr;
    }

    return &it->second;
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
        FlexibleInstance flexInstance;
        flexibleInstances.emplace(instanceId, flexInstance);
    }

    it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        return;
    }

    auto it2 = it->second.Players.find(player->GetGUID());
    if (it2 != it->second.Players.end())
    {
        // Player already exists in instance.
        return;
    }

    it->second.Players.emplace(player->GetGUID());
}

void FlexibleInstancesScript::RemovePlayerFromInstance(uint32 instanceId, Player* player)
{
    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        // Instance does not exist, no need to remove.
        return;
    }

    auto it2 = it->second.Players.find(player->GetGUID());
    if (it2 == it->second.Players.end())
    {
        // Player is not inside the instance.
        return;
    }

    it->second.Players.erase(it2);
}

uint32 FlexibleInstancesScript::GetPlayerCountForInstance(uint32 instanceId)
{
    auto it = flexibleInstances.find(instanceId);
    if (it == flexibleInstances.end())
    {
        // Instance does not exist.
        return 0;
    }

    return it->second.Players.size();
}