#include "../../includes.hpp"
#include "features.hpp"

bool updatingLby() {
    auto animState = Globals::localPlayer->animState();
    static float lbyTime = 0.0f;

    // NOTE: cmd->curtime is corrected by prediction.
    if (animState->verticalVelocity > 0.1f || fabs(animState->horizontalVelocity) > 100.0f) {
        lbyTime = Interfaces::globals->currentTime + 0.22f;

        return false;
    } else if (Interfaces::globals->currentTime > lbyTime) {
        lbyTime = Interfaces::globals->currentTime + 1.1f;

        return true;
    }

    return false;
}

void Features::AntiAim::createMove(Command *cmd) {
    if (global::health == 0) {
        return;
    }
    
    cmd->viewAngle.x = 89.0f;
    cmd->viewAngle.y += 180.0f - 58.0f;
    //cmd->viewAngle.y += (cmd->tickCount & 1 ? -1.0f : 1.0f) * jitter_for_anti_aim_state(global::anti_aim_state);

    if (global::choked <= 0) {
        global::choked = fake_lag_for_anti_aim_state(global::anti_aim_state);
        *global::send_packet = true;
    } else {
        global::choked--;
        *global::send_packet = false;
    }

    if (std::abs(cmd->move.y) < 5.0f) {
        if (updatingLby()) {
            cmd->viewAngle.y += 120.0f;

            *global::send_packet = false;
        } else if (*global::send_packet) {
            cmd->viewAngle.y += 58.0f;
        } else {
            cmd->viewAngle.y += 120.0f;
        }
    } else if (*global::send_packet) {
        cmd->viewAngle.y += 58.0f;
    } else {
        cmd->viewAngle.y += 120.0f;
    }
}
