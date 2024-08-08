#ifndef _ACTION_SCRIPT_HELPERS_H
#define _ACTION_SCRIPT_HELPERS_H

#include "Common.h"

namespace ActionScriptHelper
{
    const char* GetPlayerClassColor(Player* player);
    void AnnounceToAll(const char* message, Player* ignorePlayer = nullptr);
    std::string GetSecurityName(AccountTypes accountType);
    bool ReplaceString(std::string& s, std::string target, std::string value);
}

#endif
