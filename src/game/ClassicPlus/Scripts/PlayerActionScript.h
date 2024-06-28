#ifndef _PLAYER_ACTION_SCRIPT_H
#define _PLAYER_ACTION_SCRIPT_H

#include "ActionMgr.h"

class PlayerActionScript : public ActionScript
{
public:
    PlayerActionScript() : ActionScript("PlayerActionScript") { }

public:
    void OnPlayerUseItem(Player* player, Item* item) override
    {
        WorldPacket serverMsg(SMSG_SERVER_MESSAGE);
        serverMsg << 3; // ServerMessageType::Custom
        serverMsg << "OnPlayerUseItem";

        player->SendDirectMessage(&serverMsg);
    }

    void OnPlayerCastSpell(Player* player, Spell* spell) override
    {
        WorldPacket serverMsg(SMSG_SERVER_MESSAGE);
        serverMsg << 3; // ServerMessageType::Custom
        serverMsg << "OnPlayerCastSpell";

        player->SendDirectMessage(&serverMsg);
    }

    void OnUnitDamage(Unit* aggressor, Unit* victim, uint32& damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto, bool durabilityLoss, Spell* spell, bool reflected) override
    {
        Player* player = aggressor->ToPlayer();

        if(!player)
        {
            return;
        }

        WorldPacket serverMsg(SMSG_SERVER_MESSAGE);
        serverMsg << 3; // ServerMessageType::Custom
        serverMsg << "You damaged a unit.";

        player->SendDirectMessage(&serverMsg);
    }
};

#endif