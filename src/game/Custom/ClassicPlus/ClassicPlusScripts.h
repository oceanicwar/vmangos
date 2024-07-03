#ifndef _CUSTOM_CLASSIC_PLUS_H
#define _CUSTOM_CLASSIC_PLUS_H

#include "ActionMgr.h"

enum ClassicPlusConstants {
    CPLUS_MAP_MOLTEN_CORE = 409,

    CPLUS_MAP_DATA_FLEX = 5000
};

class ClassicPlusScripts : public ActionScript
{
public:
    ClassicPlusScripts() : ActionScript("ClassicPlusScripts") { }

public:
    void OnInitializeActionScript() override;
    void OnPlayerEnterMap(Player* player, Map* oldMap, Map* newMap) override;
    void OnCreatureUpdate(Creature* creature, uint32 update_diff, uint32 diff) override;
};

#endif