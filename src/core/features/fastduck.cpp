#include "features.hpp"

void Features::FastDuck::createMove(Command* cmd) {
    if (CONFIGBOOL("Misc>Misc>Movement>Fast Duck") && Globals::localPlayer && Globals::localPlayer->health() > 0) {
        cmd->buttons |= (1 << 22);
    }
}
