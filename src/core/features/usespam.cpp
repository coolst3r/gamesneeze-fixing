#include "features.hpp"

void Features::UseSpam::createMove(Command* cmd) {
    if (Globals::localPlayer && Globals::localPlayer->health() > 0 && cmd->tickCount % 2 == 0 && CONFIGBOOL("Misc>Misc>Misc>Use Spam") && Menu::CustomWidgets::isKeyDown(CONFIGINT("Misc>Misc>Misc>Use Spam Key"))) {
        cmd->buttons |= (1 << 5);
    }
}
