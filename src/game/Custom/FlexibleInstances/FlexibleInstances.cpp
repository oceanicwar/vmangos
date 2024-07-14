#include "FlexibleInstances.h"
#include "Custom/MetadataObject.h"

#include "Chat.h"
#include "InstanceData.h"
#include "SpellAuras.h"

void FlexibleInstancesScript::OnAfterConfigLoaded(bool reload)
{
    if (reload)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Reloading Flexible Instance Templates..");
        flexibleInstanceTemplates.clear();
    }
    else
    {
        sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Loading Flexible Instance Templates..");
    }

    uint32 count = 0;
    auto result = WorldDatabase.PQuery("SELECT map_id, player_count, hp_multi, dmg_multi, xp_multi, gold_multi, item_multi FROM flex_instance_template");

    if (result)
    {
        do
        {
            auto fields = result->Fetch();

            FlexibleInstanceTemplate flexTemplate;
            uint32 mapId = fields[0].GetUInt32();
            flexTemplate.PlayerCount = fields[1].GetUInt32();
            flexTemplate.HealthMultiplier = fields[2].GetFloat();
            flexTemplate.DamageMultiplier = fields[3].GetFloat();
            flexTemplate.ExpMultiplier = fields[4].GetFloat();
            flexTemplate.GoldMultiplier = fields[5].GetFloat();
            flexTemplate.ItemMultiplier = fields[6].GetFloat();

            auto it = flexibleInstanceTemplates.find(mapId);
            if (it == flexibleInstanceTemplates.end())
            {
                std::unordered_map<uint32, FlexibleInstanceTemplate> instanceMap;
                flexibleInstanceTemplates.emplace(mapId, instanceMap);
            }

            it = flexibleInstanceTemplates.find(mapId);
            it->second.emplace(flexTemplate.PlayerCount, flexTemplate);

            count++;
        } while (result->NextRow());
    }

    // Update the flex templates for currently loaded maps.
    if (reload)
    {
        auto maps = sMapMgr.Maps();
        for (auto& mapRef : maps)
        {
            auto map = mapRef.second;
            if (!map)
            {
                return;
            }

            if (!IsFlexibleInstance(map))
            {
                continue;
            }

            UpdateFlexTemplateForMap(map);
            NotifyFlexibilityChanged(map);
        }
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

    UpdateFlexTemplateForMap(newMap);
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

    UpdateFlexTemplateForMap(map);
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

    if (auto instanceData = map->GetInstanceData())
    {
        // Do not scale during an instance encounter.
        if (instanceData->IsEncounterInProgress())
        {
            return;
        }
    }

    auto mapTemplate = map->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);

    // The map does not have a flex template.
    if (!mapTemplate)
    {
        return;
    }

    auto creatureTemplate = creature->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);

    // The creatures template matches the map template and does not need to be updated.
    if (creatureTemplate && CheckTemplatesMatch(mapTemplate, creatureTemplate))
    {
        return;
    }

    UpdateFlexTemplateForCreature(creature);

    creatureTemplate = creature->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!creatureTemplate)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Creature '%u' flex template failed to update.", creature->GetGUID());
        return;
    }

    // Only add the special aura if the server has it patched in.
    if (sSpellMgr.GetSpellEntry(SPELL_ENTRY_FLEXIBLE))
    {
        // Update the aura stacks to match the template.
        auto aura = creature->GetAura(SPELL_ENTRY_FLEXIBLE, SpellEffectIndex::EFFECT_INDEX_0);
        if (aura)
        {
            auto stacks = aura->GetStackAmount();
            if (stacks != creatureTemplate->PlayerCount)
            {
                aura->GetHolder()->SetStackAmount(creatureTemplate->PlayerCount);
            }
        }
        else
        {
            auto holder = creature->AddAura(SPELL_ENTRY_FLEXIBLE);
            holder->SetStackAmount(creatureTemplate->PlayerCount);
        }
    }

    auto creatureInfo = creature->GetCreatureInfo();
    uint32 rank = creature->IsPet() ? 0 : creatureInfo->rank;

    auto creatureCLS = creature->GetClassLevelStats();

    float const healthMod = creature->_GetHealthMod(rank);
    uint32 const health = std::max(1u, uint32(roundf(healthMod * creatureCLS->health * creatureInfo->health_multiplier * creatureTemplate->HealthMultiplier)));

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

    auto metadata = creature->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!metadata)
    {
        return;
    }

    uint32 newDamage = std::max(1u, uint32(roundf(damage * metadata->DamageMultiplier)));
    damage = newDamage;
}

void FlexibleInstancesScript::OnUnitDamagePeriodic(Unit* aggressor, Unit* victim, uint32& damage, Aura* aura)
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

    auto metadata = creature->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!metadata)
    {
        return;
    }

    uint32 newDamage = std::max(1u, uint32(roundf(damage * metadata->DamageMultiplier)));
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

    auto metadata = creature->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!metadata)
    {
        return 0;
    }

    return std::max(1u, uint32(roundf(log->damage * metadata->DamageMultiplier)));
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

    auto metadata = creature->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!metadata)
    {
        return 0;
    }

    return std::max(1u, uint32(roundf(log->totalDamage * metadata->DamageMultiplier)));
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

    auto mapTemplate = map->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);

    // The map does not have a flex template.
    if (!mapTemplate)
    {
        return;
    }

    xp = xp * mapTemplate->ExpMultiplier;
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

    auto mapTemplate = map->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);

    // The map does not have a flex template.
    if (!mapTemplate)
    {
        return;
    }

    money = money * mapTemplate->GoldMultiplier;
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

    auto mapTemplate = map->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);

    // The map does not have a flex template.
    if (!mapTemplate)
    {
        return;
    }

    auto itemMulti = mapTemplate->ItemMultiplier;
    if (itemMulti > 1.0f || itemMulti <= 0.0f)
    {
        sLog.Out(LOG_BASIC, LOG_LVL_ERROR, ">> Item Multiplifer was set to '%f' but it must be a value between 0-1.", itemMulti);
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

const FlexibleInstanceTemplate* FlexibleInstancesScript::GetTemplateForPlayerCount(uint32 mapId, uint32 playerCount)
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

bool FlexibleInstancesScript::CheckTemplatesMatch(const FlexibleInstanceTemplate* template1, const FlexibleInstanceTemplate* template2)
{
    if (!template1 && !template2)
    {
        return true;
    }

    if ((template1 && !template2) ||
        (!template1 && template2))
    {
        return false;
    }

    float epsilon = 1e-5f;

    return template1->PlayerCount == template2->PlayerCount &&
        std::fabs(template1->HealthMultiplier - template2->HealthMultiplier) < epsilon &&
        std::fabs(template1->DamageMultiplier - template2->DamageMultiplier) < epsilon &&
        std::fabs(template1->ExpMultiplier - template2->ExpMultiplier) < epsilon &&
        std::fabs(template1->GoldMultiplier - template2->GoldMultiplier) < epsilon &&
        std::fabs(template1->ItemMultiplier - template2->ItemMultiplier) < epsilon;
}

void FlexibleInstancesScript::UpdateFlexTemplateForMap(Map* map)
{
    // Get a fitting template for the map
    uint32 playerCount = GetPlayerCountForMap(map);
    auto flexTemplate = GetTemplateForPlayerCount(map->GetId(), playerCount);

    // Copy the template to the maps metadata (replacing old template).
    map->SetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances, *flexTemplate);
}

void FlexibleInstancesScript::UpdateFlexTemplateForCreature(Creature* creature)
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

    auto mapTemplate = map->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!mapTemplate)
    {
        return;
    }

    // Copy the map template to the creature.
    creature->SetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances, *mapTemplate);
}

void FlexibleInstancesScript::NotifyFlexibilityChanged(Map* map, Player* skipPlayer)
{
    auto metadata = map->GetMetadata<FlexibleInstanceTemplate>(MetadataFlexibleInstances);
    if (!metadata)
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

        ChatHandler(player->GetSession()).PSendSysMessage("|cffFFD700[Flexible Instances]: |cffFFFFFFCreature damage, health, experience, gold, and item drop rates have been adjusted for a group of |cff00FF00%u|cffFFFFFF.|r", metadata->PlayerCount);
    }
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

uint32 FlexibleInstancesScript::GetPlayerCountForMap(Map* map)
{
    if (!map)
    {
        return 0;
    }

    uint32 count = 0;
    Map::PlayerList const& players = map->GetPlayers();
    for (const auto& playerRef : players)
    {
        auto player = playerRef.getSource();
        if (!player)
        {
            continue;
        }

        count++;
    }

    return count;
}