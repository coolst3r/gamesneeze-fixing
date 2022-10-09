#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <dlfcn.h>
#include <SDL2/SDL.h>
#include <cstdint>

#include "core/menu/imgui/imgui.h"
#include "core/menu/imgui/GL/gl3w.h"
#include "core/menu/imgui/imgui_impl_sdl.h"
#include "core/menu/imgui/imgui_impl_opengl3.h"

#include "core/menu/config.hpp"
#include "utils/utils.hpp"
#include "sdk/sdk.hpp"
#include "core/features/features.hpp"
#include "core/hooks/hooks.hpp"
#include "core/menu/menu.hpp"

namespace Globals {
    inline VMatrix worldToScreenMatrix;
    inline int screenSizeX = 0;
    inline int screenSizeY = 0;
    inline ImDrawList* drawList = nullptr;
    inline float FOV = 90.0;

    inline Player* localPlayer = nullptr;
    inline QAngle oldViewAngle = QAngle{0, 0, 0};
   
    inline QAngle cmdViewAngle = QAngle{0, 0, 0};
    inline QAngle viewAngle = QAngle{0, 0, 0};
}

namespace global {
    inline AntiAimState anti_aim_state;

    inline Player *local{nullptr};
    inline Weapon *weapon{nullptr};
    inline bool *send_packet{nullptr};

    inline int ammo{0};
    inline int choked{0};
    inline int health{0};
    inline int tick_count{0};

    inline float side{1.0};
    inline float use_shoot_view_angle{0.0};

    inline QAngle view_angle{0, 0, 0};
    inline QAngle shoot_view_angle{0, 0, 0};
    inline QAngle sent_view_angle{0, 0, 0};
    inline Vector eye_pos{};
}

// Console variables/convars
namespace Vars {
    inline ConVar *cl_bob_lower_amt = nullptr;
    inline ConVar *cl_bobamt_lat = nullptr;
    inline ConVar *cl_bobamt_vert = nullptr;
    inline ConVar *cl_bobcycle = nullptr;
    inline ConVar *cl_competitive_minspec = nullptr;
    inline ConVar *cl_csm_enabled = nullptr;
    inline ConVar *cl_interp = nullptr;
    inline ConVar *cl_interp_ratio = nullptr;
    inline ConVar *cl_interpolate = nullptr;
    inline ConVar *cl_lagcompensation = nullptr;
    inline ConVar *cl_phys_timescale = nullptr;
    inline ConVar *cl_ragdoll_gravity = nullptr;
    inline ConVar *cl_windspeed = nullptr;
    inline ConVar *cl_wpn_sway_scale = nullptr;
    inline ConVar *cl_viewmodel_shift_left_amt = nullptr;
    inline ConVar *cl_viewmodel_shift_right_amt = nullptr;

    inline ConVar *engine_no_focus_sleep = nullptr;
    
    inline ConVar *host_timescale = nullptr; 

    inline ConVar *mat_drawgray = nullptr; 
    inline ConVar *mat_fullbright = nullptr;
    inline ConVar *mat_postprocess_enable = nullptr;
    inline ConVar *mat_showlowresimage = nullptr;

    inline ConVar *mp_teammates_are_enemies = nullptr;

    inline ConVar *name = nullptr;
    
    inline ConVar *net_fakelag = nullptr; 

    inline ConVar *panorama_disable_blur = nullptr;

    inline ConVar *r_3dsky = nullptr;
    inline ConVar *r_aspectratio = nullptr;
    inline ConVar *r_rainlength = nullptr;
    inline ConVar *r_rainspeed = nullptr;
    inline ConVar *r_rainradius = nullptr;
    inline ConVar *r_rainwidth = nullptr;
    inline ConVar *r_rainalpha = nullptr;
    inline ConVar *r_RainSideVel = nullptr;
    inline ConVar *r_drawmodelstatsoverlay = nullptr;

    inline ConVar *sv_cheats = nullptr;
    inline ConVar *sv_gravity = nullptr;
    inline ConVar *sv_infinite_ammo = nullptr;
    inline ConVar *sv_showimpacts = nullptr;

    inline ConVar *viewmodel_fov = nullptr;
    inline ConVar *viewmodel_offset_x = nullptr;
    inline ConVar *viewmodel_offset_y = nullptr;
    inline ConVar *viewmodel_offset_z = nullptr;

    inline ConVar *weapon_recoil_scale = nullptr;
}
