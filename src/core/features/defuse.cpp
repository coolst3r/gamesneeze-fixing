#include "../../includes.hpp"
#include "features.hpp"

void Features::Defuse::createMoveStart(Command *cmd) {
    auto autoDefuse = CONFIGBOOL("Misc>Misc>Misc>Auto Defuse");
    auto silentDefuse = CONFIGBOOL("Misc>Misc>Misc>Silent Defuse");
    auto antiAiming = CONFIGINT("Rage>AntiAim>Type") != 0;

    if (!(autoDefuse || silentDefuse || antiAiming)) {
        return;
    }
    
    if (Features::Defuse::performDefuse) {
        cmd->buttons |= IN_USE;
    }

    if ((cmd->buttons & IN_USE) != 0 && Features::Defuse::canDefuse && (silentDefuse || antiAiming)) {
        cmd->viewAngle = Features::Defuse::angles;
    }
}

void Features::Defuse::createMoveEnd(Command *cmd) {
    Features::Defuse::canDefuse = false;
    Features::Defuse::performDefuse = false;
}

void Features::Defuse::onBombRender(PlantedC4 *bomb) {
    auto autoDefuse = CONFIGBOOL("Misc>Misc>Misc>Auto Defuse");
    auto silentDefuse = CONFIGBOOL("Misc>Misc>Misc>Silent Defuse");
    auto antiAiming = CONFIGINT("Rage>AntiAim>Type") != 0;

    if (!(autoDefuse || silentDefuse || antiAiming)) {
        Features::Defuse::canDefuse = false;
        Features::Defuse::performDefuse = false;

        return;
    }

    auto playerOrigin = Globals::localPlayer->origin();
    auto bombOrigin = bomb->origin();

    if (getDistanceNoSqrt(playerOrigin, bombOrigin) < 5625) {
        Features::Defuse::canDefuse = true;
    } else {
        Features::Defuse::canDefuse = false;
    }

    if (silentDefuse || antiAiming) {
        auto eyePos = Globals::localPlayer->eyePos();
        auto angles = calcAngle(eyePos, bombOrigin);

        clampAngle(angles);

        Features::Defuse::angles = angles;
    }

    if (CONFIGBOOL("Misc>Misc>Misc>Latest Defuse")) {
        auto timeRemaining = bomb->time() - (Interfaces::globals->currentTime + ((float)playerResource->GetPing(Globals::localPlayer->index()) / 1000.f));

        if (timeRemaining < (Globals::localPlayer->defuser() ? 5.1f: 10.1f)) {
            Features::Defuse::performDefuse = true;

            return;
        }
    } else {
        Features::Defuse::performDefuse = true;
    }

    Features::Defuse::performDefuse = false;
}
