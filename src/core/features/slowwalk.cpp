#include "features.h"
#include "../../includes.hpp"

void Features::SlowWalk::createMove(Command *cmd) {
    if (Interfaces::engine->IsInGame() && Globals::localPlayer && Globals::localPlayer->health() > 0 && 
            (Globals::localPlayer->flags() & IN_ATTACK) && (cmd->buttons & IN_SPEED) && CONFIGBOOL("Rage>AntiAim>Slow Walk") &&
            (Globals::localPlayer->velocity().Length2D() > CONFIGINT("Rage>AntiAim>Slow Walk Speed"))) {
        cmd->move.x = 0;
        cmd->move.y = 0;
    }
}
