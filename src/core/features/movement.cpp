#include "../../includes.hpp"
#include "features.hpp"

void bhop(Command* cmd) {
    if (!CONFIGBOOL("Misc>Misc>Movement>Bhop")) {
        return;
    }
    
    if (Globals::localPlayer->moveType() == MOVETYPE_LADDER || Globals::localPlayer->moveType() == MOVETYPE_NOCLIP) {
        return;
    }

    if (CONFIGBOOL("Misc>Misc>Movement>Humanised Bhop")) {
        // https://www.unknowncheats.me/forum/counterstrike-global-offensive/333797-humanised-bhop.html
        static int hopsRestricted = 0;
        static int hopsHit = 0;

        if (!Globals::localPlayer->on_ground()) {
            cmd->buttons &= ~IN_JUMP;
            hopsRestricted = 0;
        } else if ((rand() % 100 > CONFIGINT("Misc>Misc>Movement>Bhop Hitchance")
            && hopsRestricted < CONFIGINT("Misc>Misc>Movement>Bhop Max Misses"))
            || (CONFIGINT("Misc>Misc>Movement>Bhop Max Hops Hit") > 0
            && hopsHit > CONFIGINT("Misc>Misc>Movement>Bhop Max Hops Hit"))) {
            cmd->buttons &= ~IN_JUMP;
            hopsRestricted++;
            hopsHit = 0;
        } else {
            hopsHit++;
        }
    } else if (!Globals::localPlayer->on_ground()) {
        cmd->buttons &= ~IN_JUMP;
    } else if (
        CONFIGBOOL("Misc>Misc>Movement>Auto Bhop")
    && ((cmd->buttons & IN_FORWARD) != 0
        || (cmd->buttons & IN_BACK) != 0
        || (cmd->buttons & IN_MOVELEFT) != 0
        || (cmd->buttons & IN_MOVERIGHT) != 0)
    && (cmd->buttons & IN_DUCK) == 0
    && (cmd->buttons & IN_SPEED) == 0) {
        cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);
        cmd->buttons |= IN_JUMP;
    }
}

void Features::Movement::backupAngle(Command *cmd) {
    Features::Movement::anglesBackup = cmd->viewAngle;
}

void Features::Movement::restoreAngle(Command *cmd) {
    cmd->viewAngle = Features::Movement::anglesBackup;
}

enum Direction {
    Forward = 0,
    Backward = -180,
    Left = 90,
    Right = -90,
};

void legitStrafe(Command *cmd) {
    if ((cmd->buttons & IN_FORWARD) != 0
     || (cmd->buttons & IN_BACK) != 0
     || (cmd->buttons & IN_MOVELEFT) != 0
     || (cmd->buttons & IN_MOVERIGHT) != 0) {
        return;
    }

    if (cmd->mouseX <= 1.0f && cmd->mouseX >= -1.0f) {
        return;
    }
	
    cmd->move.y = cmd->mouseX < 0.0f ? -250.0f : 250.0f;
}

inline bool inTransition = false;
inline QAngle targetAngle = QAngle{};

void directionalStrafe(Command *cmd) {
    static float side = 1.f;
    side = -side;

    const Vector &velocity = Globals::localPlayer->velocity();
    float idealStrafe =
         std::clamp(sdk::to_degrees(atan(15.f / velocity.Length2D())), 0.f, 90.f);

    QAngle wishAngles = cmd->viewAngle;
    Vector strafeDir = Vector(cmd->move.x, cmd->move.y, 0.f);
    strafeDir.Normalize();
    float strafeDirYawOffset = sdk::to_degrees(atan2f(strafeDir.y, strafeDir.x));
    wishAngles.y -= strafeDirYawOffset;
    sanitizeAngle(wishAngles);
    static float oldYaw = 0.f;
    float yawDelta = std::remainderf(wishAngles.y - oldYaw, 360.f);
    oldYaw = wishAngles.y;
    
    static ConVar *cl_sidespeed = Interfaces::convar->FindVar("cl_sidespeed");

    if (abs(yawDelta) <= idealStrafe || abs(yawDelta) >= 30.f) {
        QAngle veloDir;
        vectorAngle(velocity, veloDir);
        float veloYawDelta = std::remainderf(wishAngles.y - veloDir.y, 360.f);
        float retrack =
             std::clamp(sdk::to_degrees(atan(30.f / velocity.Length2D())), 0.f, 90.f) * 2.f;
        if (veloYawDelta <= retrack || velocity.Length2D() <= 15.f) {
            if (-retrack <= veloYawDelta || velocity.Length2D() <= 15.f) {
                wishAngles.y += side * idealStrafe;
                cmd->move.y = cl_sidespeed->GetFloat() * side;
            } else {
                wishAngles.y = veloDir.y - retrack;
                cmd->move.y = cl_sidespeed->GetFloat();
            }
        } else {
            wishAngles.y = veloDir.y + retrack;
            cmd->move.y = -cl_sidespeed->GetFloat();
        }
    } else if (yawDelta > 0.f)
        cmd->move.y = -cl_sidespeed->GetFloat();
    else if (yawDelta != 0.f)
        cmd->move.y = cl_sidespeed->GetFloat(); 

    cmd->move.x = 0.f;

    QAngle viewBackup = cmd->viewAngle;
    cmd->viewAngle = wishAngles;
    startMovementFix(cmd);
    cmd->viewAngle = viewBackup;
    endMovementFix(cmd);
}

void Features::Movement::autoStrafe(Command *cmd) {
    if (!CONFIGBOOL("Misc>Misc>Movement>Auto Strafe")) {
        return;
    }

    if (Globals::localPlayer == nullptr) {
        return;
    }
    
    if (Globals::localPlayer->health() == 0) {
        return;
    }
    
    if (Globals::localPlayer->moveType() == MOVETYPE_LADDER || Globals::localPlayer->moveType() == MOVETYPE_NOCLIP) {
        return;
    }
    
    if (Globals::localPlayer->on_ground()) {
        return;
    }    
    
    if (CONFIGBOOL("Rage>Enabled")) {
        directionalStrafe(cmd);
    } else {
        legitStrafe(cmd);
    }
}


void edgeJump(Command *cmd) {
    if (CONFIGBOOL("Misc>Misc>Movement>Edge Jump")
        && Menu::CustomWidgets::isKeyDown(CONFIGINT("Misc>Misc>Movement>Edge Jump Key"))
        && (Features::Movement::flagsBackup & sdk::player_flags::ON_GROUND)
        && !Globals::localPlayer->on_ground()) {
        cmd->buttons |= IN_JUMP;
    }
}

void jumpBug(Command *cmd) {
    static bool shouldSkip = false;

    if (shouldSkip) {
        shouldSkip = false;
        return;
    }

    if (CONFIGBOOL("Misc>Misc>Movement>JumpBug")
    && Menu::CustomWidgets::isKeyDown(CONFIGINT("Misc>Misc>Movement>JumpBug Key"))
    && !((Features::Movement::flagsBackup & sdk::player_flags::ON_GROUND)
        || (Features::Movement::flagsBackup & sdk::player_flags::ON_PARTIAL_GROUND))
    && (Globals::localPlayer->on_ground()
        || Globals::localPlayer->on_partial_ground())) {
        cmd->buttons |= IN_DUCK;
        cmd->buttons &= ~IN_JUMP;
        shouldSkip = true;
    }
}

bool checkEdgebug() {
    auto gravity = Vars::sv_gravity->GetFloat();
    auto edgebugZVel = gravity * 0.5f * Interfaces::globals->intervalPerTick;

    return Features::Movement::velBackup.z < -edgebugZVel &&
           round(Globals::localPlayer->velocity().z) == -round(edgebugZVel) &&
           Globals::localPlayer->moveType() != MOVETYPE_LADDER;
}

void Features::Movement::prePredCreateMove(Command *cmd) {
    if (Globals::localPlayer == nullptr) {
        return;
    }

    flagsBackup = Globals::localPlayer->flags();
    velBackup = Globals::localPlayer->velocity();

    bhop(cmd);

    if (shouldEdgebug && shouldDuckNext) {
        cmd->buttons |= IN_DUCK;
    }
}

void Features::Movement::postPredCreateMove(Command *cmd) {
    if (!Globals::localPlayer) {
        return;
    }

    if (Globals::localPlayer->moveType() == MOVETYPE_LADDER
     || Globals::localPlayer->moveType() == MOVETYPE_NOCLIP) {
        return;
    }

    edgeJump(cmd);
    jumpBug(cmd);
}

void Features::Movement::edgeBugPredictor(Command *cmd) {
    if (!CONFIGBOOL("Misc>Misc>Movement>EdgeBug")
     || !Menu::CustomWidgets::isKeyDown(CONFIGINT("Misc>Misc>Movement>EdgeBug Key"))
     || !Globals::localPlayer->health()) {
        return;
    }

    if (Globals::localPlayer->flags() & FL_ONGROUND || Globals::localPlayer->moveType() == MOVETYPE_LADDER) {
        return;
    }

    struct MovementVars {
        QAngle viewAngle;
        QAngle viewDelta;
        float forwardMove;
        float sideMove;
        int buttons;
    };

    static MovementVars backupMove;
    MovementVars originalMove;

    originalMove.viewAngle = cmd->viewAngle;
    originalMove.viewDelta = (cmd->viewAngle - Globals::oldViewAngle);
    originalMove.forwardMove = cmd->move.x;
    originalMove.sideMove = cmd->move.y;
    originalMove.buttons = cmd->buttons;

    if (!shouldEdgebug) {
        backupMove = originalMove;
    }

    int nCmdsPred = Interfaces::prediction->Split->nCommandsPredicted;
    // TODO: make amount configurable
    int predictAmount = 128;
    
    for (int t = 0; t < 4; ++t) {
        Features::Prediction::restoreEntityToPredictedFrame(nCmdsPred - 1);

        bool doStrafe = (t % 2 == 0);
        bool doDuck = t > 1;

        cmd->viewAngle = backupMove.viewAngle;

        for (int i = 0; i < predictAmount; i++) {
            if (doStrafe) {
                cmd->viewAngle += backupMove.viewDelta;
                cmd->move.x = backupMove.forwardMove;
                cmd->move.y = backupMove.sideMove;
            } else {
                cmd->move.x = 0.0f;
                cmd->move.y = 0.0f;
            }

            cmd->buttons ^= (-doDuck ^ cmd->buttons) & IN_DUCK;

            Features::Prediction::start(cmd);
            shouldEdgebug = checkEdgebug();
            velBackup = Globals::localPlayer->velocity();
            edgebugPos = Globals::localPlayer->origin();
            Features::Prediction::end();


            if (shouldEdgebug) {
                shouldDuckNext = doDuck;

                if (doStrafe) {
                    cmd->viewAngle = backupMove.viewAngle + backupMove.viewDelta;
                    backupMove.viewAngle = cmd->viewAngle;
                }

                if (i == 1) {
                    // TODO: play sound via a better method
                    Interfaces::engine->ExecuteClientCmd("play buttons/blip1.wav");
                }

                return;
            }
        }
    }

    cmd->viewAngle = originalMove.viewAngle;
    cmd->move.x = originalMove.forwardMove;
    cmd->move.y = originalMove.sideMove;
    cmd->buttons = originalMove.buttons;
}

void Features::Movement::draw() {
    if (Features::Movement::shouldEdgebug) {
        Globals::drawList->AddText(
           ImVec2((Globals::screenSizeX / 2) - (ImGui::CalcTextSize("EdgeBug").x / 2) + 1,
                  (Globals::screenSizeY / 2) + 31),
           ImColor(0, 0, 0, 255), "EdgeBug");
        Globals::drawList->AddText(
           ImVec2((Globals::screenSizeX / 2) - (ImGui::CalcTextSize("EdgeBug").x / 2),
                  (Globals::screenSizeY / 2) + 30),
           ImColor(255, 255, 255, 255), "EdgeBug");

        Vector edgebugPos2D;
        if (worldToScreen(edgebugPos, edgebugPos2D)) {
            Globals::drawList->AddText(
               ImVec2(edgebugPos2D.x - (ImGui::CalcTextSize("gaming").x / 2) + 1,
                      edgebugPos2D.y + 1),
               ImColor(0, 0, 0, 255), "gaming");
            Globals::drawList->AddText(
               ImVec2(edgebugPos2D.x - (ImGui::CalcTextSize("gaming").x / 2),
                      edgebugPos2D.y),
               ImColor(255, 255, 255, 255), "gaming");
        }
    }
}
