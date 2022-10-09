#include "features.hpp"
#include "../../sdk/sdk.hpp"

 void Features::NoVisualRecoil::frameStageNotify(FrameStage frame) {
    if (frame != FRAME_RENDER_START) {
       return;
    }

    if (Globals::localPlayer == nullptr) {
       return;
    }
      
    Features::NoVisualRecoil::aimPunch = Globals::localPlayer->aimPunch();

    if (CONFIGBOOL("Visuals>Players>LocalPlayer>No Aim Punch")) {
        Globals::localPlayer->aimPunch() = QAngle{};
    }

    if (CONFIGBOOL("Visuals>Players>LocalPlayer>No View Punch")) {
        Globals::localPlayer->viewPunch() = QAngle{};
    }
}
