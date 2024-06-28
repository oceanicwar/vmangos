#ifndef _ACTIONMGR_H
#define _ACTIONMGR_H

#include "Item.h"
#include "Player.h"
#include "Policies/Singleton.h"

class ActionScript
{
public:
    ActionScript(const char* name) 
    {
        _name = name;
    }

    virtual void OnPlayerUseItem(Player* player, Item* item) { }

    const char* GetName()
    {
        return _name;
    }

private:
    const char* _name;
};

enum ActionTypes : uint32
{
    ACTION_ON_PLAYER_USE_ITEM = 0,
    ACTION_TYPES_END = 1
};

class ActionMgr
{
public:
    ActionMgr();
    ~ActionMgr();

    void Inititalize();
    void RegisterActions(ActionScript* script, std::vector<uint32> actions);

    void ActionOnPlayerUseItem(Player* player, Item* item);

private:
    std::map<uint32, std::vector<ActionScript*>> actionScripts;
};

#define sActionMgr MaNGOS::Singleton<ActionMgr>::Instance()

#endif