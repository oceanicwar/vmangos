#include "ClassicPlusScripts.h"

#include "InstanceData.h"

void ClassicPlusScripts::OnInitializeActionScript()
{
    // TODO: Load flexible templates here.
}

void ClassicPlusScripts::OnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap)
{
    if (!player || !newMap)
    {
        return;
    }

    uint32 mapId = newMap->GetId();
    uint32 oldMapId = oldMap ? oldMap->GetId() : 1337;

    sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Player '%s' entered map '%u' from old map '%u'.", player->GetName(), mapId, oldMapId);
}

void ClassicPlusScripts::OnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff)
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

    if (!map->IsDungeon() && !map->IsRaid())
    {
        return;
    }

    // TODO: Modify creatures stats based on flexible scaling level
}