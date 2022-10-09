#include "features.hpp"
#include <cstring>

void Features::Variables::frameStageNotify(FrameStage frame) {
    auto cheats = CONFIGBOOL("Misc>Misc>Variables>Cheats");
    auto fullbright = CONFIGBOOL("Visuals>World>World>Fullbright");
    auto draw_gray = CONFIGBOOL("Visuals>World>World>Gray");
    auto host_timescale = static_cast<float>(CONFIGINT("Misc>Misc>Variables>Timescale")) / 1000.0f;
    auto infinite_ammo = CONFIGBOOL("Misc>Misc>Variables>Infinite Ammo");
    auto low_resolution = CONFIGBOOL("Visuals>World>World>Low-res Image");
    auto no_shadows = !CONFIGBOOL("Visuals>World>World>No Shadows");
    auto post_processing = !CONFIGBOOL("Misc>Misc>Misc>Disable Post Processing");
    auto panorama_disable_blur = CONFIGBOOL("Misc>Misc>Misc>Disable Panorama Blur");
    auto rage = !CONFIGBOOL("Rage>Enabled");
    auto ragdoll_gravity = CONFIGBOOL("Visuals>World>World>Ragdoll Gravity") ? -600 : 600;
    auto show_impacts = std::clamp(CONFIGINT("Visuals>World>World>Show Impacts"), 0, 3);
    auto model_chams = CONFIGINT("Visuals>World>World>Model Material") != 0 ? 2 : 0;

    switch (frame) {
        case FRAME_RENDER_START: {
            Vars::cl_csm_enabled->SetValue(no_shadows);
            Vars::cl_ragdoll_gravity->SetValue(ragdoll_gravity);

            Vars::engine_no_focus_sleep->SetValue(!rage);

            Vars::host_timescale->SetValue(host_timescale);

            // TODO: use the variable not the cvar!
            Vars::mat_postprocess_enable->SetValue(post_processing);
    
            Vars::panorama_disable_blur->SetValue(panorama_disable_blur);

            Vars::r_drawmodelstatsoverlay->SetValue(model_chams);

            Vars::sv_cheats->SetValue(cheats);
            Vars::sv_infinite_ammo->SetValue(infinite_ammo);
            Vars::sv_showimpacts->SetValue(show_impacts);

            break;
        }
        case FRAME_RENDER_END: {
            //Vars::cl_csm_enabled->SetValue(0);
            //Vars::cl_ragdoll_gravity->SetValue(800);
            //Vars::r_drawmodelstatsoverlay->SetValue(0);
            //Vars::sv_cheats->SetValue(0);
            //Vars::sv_infinite_ammo->SetValue(2);
            //Vars::sv_showimpacts->SetValue(0);

            break;
        }
        default: {
            break;
        }
    }
}
