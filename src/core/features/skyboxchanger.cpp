#include "features.hpp"
#include <cstring>

void Features::SkyboxChanger::frameStageNotify(FrameStage frame) {
    if (frame == FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
        if (CONFIGINT("Visuals>World>World>Skybox")) {
            Offsets::setNamedSkybox((CONFIGINT("Visuals>World>World>Skybox") != 1) ? skyboxes[CONFIGINT("Visuals>World>World>Skybox")] : "sky_l4d_rural02_ldr");

            Vars::r_3dsky->SetValue(0);
        } else {
            Vars::r_3dsky->SetValue(1);
        }
    }
}
