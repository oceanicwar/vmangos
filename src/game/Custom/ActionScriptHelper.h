#ifndef _ACTION_SCRIPT_HELPERS_H
#define _ACTION_SCRIPT_HELPERS_H

namespace ActionScriptHelper
{
    const char* GetPlayerClassColor(Player* player);
    void AnnounceToAll(const char* message, Player* ignorePlayer = nullptr);
}

#endif