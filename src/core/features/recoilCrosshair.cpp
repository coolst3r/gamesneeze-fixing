#include "features.hpp"
#include "../../includes.hpp"

inline float spread = 0.0f;
inline float inaccuracy = 0.0f;

void Features::RecoilCrosshair::draw() {
    if (!Interfaces::engine->IsInGame()) {
        return;
    }

    if (Globals::localPlayer == nullptr) {
        return;
    }

    if (Globals::localPlayer->health() == 0) {
        return;
    }

    auto recoilCrosshair = CONFIGBOOL("Visuals>Players>LocalPlayer>Recoil Crosshair");
    auto onlyWhenShooting = CONFIGBOOL("Visuals>Players>LocalPlayer>Recoil Crosshair>Only When Shooting");
    auto spreadCrosshair = CONFIGBOOL("Visuals>Players>LocalPlayer>Spread Crosshair");
    
    if (!(recoilCrosshair || spreadCrosshair)) {
        return;
    }

    auto radians = 0.0f;
    auto x = static_cast<float>(Globals::screenSizeX / 2);
    auto y = static_cast<float>(Globals::screenSizeY / 2);
    auto dX = static_cast<float>(Globals::screenSizeX / Globals::FOV);
    auto dY = static_cast<float>(Globals::screenSizeY / Globals::FOV);

    if (recoilCrosshair) {
        radians = 5.0f;
        x = x - (dX * Features::NoVisualRecoil::aimPunch.y);
        y = y + (dY * Features::NoVisualRecoil::aimPunch.x);
    }

    if (spreadCrosshair) {
        radians = ((spread + inaccuracy) * Globals::screenSizeY) / 1.5f;
    }

    if (onlyWhenShooting && Features::NoVisualRecoil::aimPunch.IsZero()) {
        return;
    }
    
    auto borderColor = CONFIGCOL("Visuals>Players>LocalPlayer>Crosshair Border Color");
    auto crosshairColor = CONFIGCOL("Visuals>Players>LocalPlayer>Crosshair Color");

    if (recoilCrosshair && !spreadCrosshair) {
        Globals::drawList->AddRectFilled({x - 4, y - 1}, {x + 5, y + 2}, borderColor);
        Globals::drawList->AddRectFilled({x - 1, y - 4}, {x + 2, y + 5}, borderColor);
        Globals::drawList->AddLine({x - 3, y}, {x + 4, y}, crosshairColor);
        Globals::drawList->AddLine({x, y + 3}, {x, y - 4}, crosshairColor);
    } else {
        Globals::drawList->AddCircleFilled({x, y}, radians, crosshairColor);
        Globals::drawList->AddCircle({x, y}, radians, borderColor);
    }
}

void Features::RecoilCrosshair::frameStageNotify(FrameStage frame) {
    if (frame != FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
        return;
    }

    if (!Interfaces::engine->IsInGame()) {
        return;
    }

    if (Globals::localPlayer == nullptr) {
        return;
    }

    if (Globals::localPlayer->health() == 0) {
        return;
    }

    auto recoilCrosshair = CONFIGBOOL("Visuals>Players>LocalPlayer>Recoil Crosshair");
    auto spreadCrosshair = CONFIGBOOL("Visuals>Players>LocalPlayer>Spread Crosshair");

    if (!(recoilCrosshair || spreadCrosshair)) {
        return;
    }

    auto weapon = Interfaces::entityList->activeWeapon(Globals::localPlayer);

    if (weapon == nullptr) {
        return;
    }

    spread = weapon->spread();
    inaccuracy = weapon->inaccuracy();
}
