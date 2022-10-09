#include "../../includes.hpp"
#include "hooks.hpp"

void Hooks::FrameStageNotify::hook(void *thisptr, FrameStage frame) {
    global::local = Interfaces::entityList->player(Interfaces::engine->GetLocalPlayer());
    Globals::localPlayer = global::local;
    global::view_angle = Interfaces::engine->viewAngle();

    if (global::local != nullptr) {
        global::anti_aim_state = !Globals::localPlayer->on_ground() ?
            AntiAimState::InAir :
                Globals::localPlayer->velocity().Length2D() > 0.0 ?
                AntiAimState::Walking :
                AntiAimState::Freestanding;

        global::eye_pos = global::local->eyePos();
        global::health = global::local->health();
        global::weapon = Interfaces::entityList->activeWeapon(global::local);
    } else {
        global::anti_aim_state = AntiAimState::Freestanding;
        global::eye_pos = Vector{};
        global::health = 0;
        global::weapon = nullptr;
    }
    
    if (global::weapon != nullptr) {
        global::ammo = global::weapon->ammo();
    } else {
        global::ammo = 0;
    }

    Features::ClantagChanger::frameStageNotify(frame);
    Features::SkyboxChanger::frameStageNotify(frame);
    Features::RecoilCrosshair::frameStageNotify(frame);
    Features::SkinChanger::frameStageNotify(frame);
    Features::NoFlash::frameStageNotify(frame);
    Features::NoVisualRecoil::frameStageNotify(frame);
    Features::Variables::frameStageNotify(frame);
    Features::Resolver::frameStageNotify(frame);

    if (frame == FRAME_RENDER_START) {
        cachePlayers();

        if (Globals::localPlayer != nullptr) {
            if (Interfaces::input->thirdperson) {
                *Globals::localPlayer->viewAngle() = global::sent_view_angle;
            } else {
                if (global::use_shoot_view_angle != 0.0) {
                    if (global::use_shoot_view_angle > Interfaces::globals->currentTime) {
                        *Globals::localPlayer->viewAngle() = global::shoot_view_angle;
                    } else {
                        global::use_shoot_view_angle = 0.0;
                        *Globals::localPlayer->viewAngle() = global::view_angle;
                    }
                }

                Globals::localPlayer->viewAngle()->z = 15.0f;
            }
        }
    }

    original(thisptr, frame);
}
