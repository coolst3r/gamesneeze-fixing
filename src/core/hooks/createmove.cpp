#include "../../includes.hpp"
#include "hooks.hpp"
#include <algorithm>
#include <cstdint>

bool Hooks::CreateMove::hook(void *self, float inputSampleTime, Command *cmd) {
    original(self, inputSampleTime, cmd);

    if (cmd->tickCount == 0) {
        return false;
    }

    if (global::health == 0) {
        global::send_packet = nullptr;
        global::choked = 0;

        return false;
    }

    uintptr_t *rbp;	
    asm volatile("mov %%rbp, %0" : "=r" (rbp));
    global::send_packet = ((*(bool **)rbp) - (int)24);
    global::tick_count = cmd->tickCount;
    //global::side = global::tick_count % 4 >= 2 ? 1.0 : -1.0;

    Globals::viewAngle = Interfaces::engine->viewAngle();

    Features::Movement::autoStrafe(cmd);
    Features::Movement::backupAngle(cmd);

    startMovementFix(cmd);

    Features::RankReveal::createMove(cmd);
    Features::FastDuck::createMove(cmd);
    Features::UseSpam::createMove(cmd);
    Features::Movement::prePredCreateMove(cmd);

    Features::Prediction::start(cmd);

    auto rage = CONFIGBOOL("Rage>Enabled");
    auto ragebot = CONFIGBOOL("Rage>RageBot>Default>Autowall");

    if (rage) {
        Features::AntiAim::createMove(cmd);
    }

    Features::Defuse::createMoveStart(cmd);
    Features::Backtrack::store(cmd);
    Features::Backtrack::createMove(cmd);
    Features::Forwardtrack::createMove(cmd);

    if (ragebot) {
        if (Features::RageBot::createMove(cmd)) {
            global::use_shoot_view_angle = Interfaces::globals->currentTime + 0.2;
            global::shoot_view_angle = cmd->viewAngle;
        }
    }
        
    Features::Prediction::end();
    Features::Movement::postPredCreateMove(cmd);

    endMovementFix(cmd);

    Features::SlowWalk::createMove(cmd);

    auto viewBackup = cmd->viewAngle;

    Features::Movement::edgeBugPredictor(cmd);

    startMovementFix(cmd);
    cmd->viewAngle = viewBackup;
    endMovementFix(cmd);

    cmd->move.x = std::clamp(cmd->move.x, -450.0f, 450.0f);
    cmd->move.y = std::clamp(cmd->move.y, -450.0f, 450.0f);
    cmd->move.z = 0.0f;

    normalizeAngle(cmd->viewAngle);
    clampAngle(cmd->viewAngle);

    Globals::oldViewAngle = cmd->viewAngle;

    if (*global::send_packet) {
        global::sent_view_angle = cmd->viewAngle;
    }
    
    // return false when we want to do silent angles for rage
    return false;
}
