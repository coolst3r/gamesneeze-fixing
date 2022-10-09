#include "../../includes.hpp"
#include "icvar.hpp"
#include "igameevent.hpp"
#include "interfaces.hpp"
#include <cstdint>
#include <string>

typedef IClientMode* (*getClientModeFunc)();

template <typename T>
static constexpr auto relativeToAbsolute(std::uintptr_t address) noexcept {
    return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

bool Interfaces::init() {
    Log::log(LOG, "Initialising interfaces...");

    convar = getInterface<ICvar>("./bin/linux64/materialsystem_client.so", "VEngineCvar"); // do this first so we can log all the interfaces to game console
    client = getInterface<IBaseClientDLL>("./csgo/bin/linux64/client_client.so", "VClient");
    engine = getInterface<IVEngineClient>("./bin/linux64/engine_client.so", "VEngineClient");
    panel = getInterface<IPanel>("./bin/linux64/vgui2_client.so", "VGUI_Panel");
    entityList = getInterface<IClientEntityList>("./csgo/bin/linux64/client_client.so", "VClientEntityList");
    engineVgui = getInterface<IEngineVGui>("./bin/linux64/engine_client.so", "VEngineVGui");
    modelRender = getInterface<IVModelRender>("./bin/linux64/engine_client.so", "VEngineModel");
    modelInfo = getInterface<IVModelInfo>("./bin/linux64/engine_client.so", "VModelInfoClient");
    materialSystem = getInterface<IMaterialSystem>("./bin/linux64/materialsystem_client.so", "VMaterialSystem");
    sound = getInterface<IEngineSound>("./bin/linux64/engine_client.so", "IEngineSoundClient");
    trace = getInterface<IEngineTrace>("./bin/linux64/engine_client.so", "EngineTraceClient");
	movement = getInterface<IGameMovement>("./csgo/bin/linux64/client_client.so", "GameMovement");
    physics = getInterface<IPhysicsSurfaceProps>("./bin/linux64/vphysics_client.so", "VPhysicsSurfaceProps");
    prediction = getInterface<IPrediction>("./csgo/bin/linux64/client_client.so", "VClientPrediction001", true);
    eventManager = getInterface<IGameEventManager2>("./bin/linux64/engine_client.so", "GAMEEVENTSMANAGER002", true);

    /* Get IClientMode */
    uintptr_t HudProcessInput = getVirtualFunc<uintptr_t>(reinterpret_cast<void *>(client), 10);
	getClientModeFunc getClientMode = reinterpret_cast<getClientModeFunc>(getAbsoluteAddress(HudProcessInput + 11, 1, 5));
    clientMode = getClientMode();
    Log::log(LOG, " ClientMode %lx", (uintptr_t)clientMode);

    /* Get globals */
    uintptr_t hudUpdate = getVirtualFunc<uintptr_t>(reinterpret_cast<void *>(client), 11);
	globals = *reinterpret_cast<GlobalVars**>(getAbsoluteAddress(hudUpdate + 13, 3, 7));
    Log::log(LOG, " Globals %lx", (uintptr_t)globals);

    /* Get input (for thirdperson) */
    uintptr_t activateMouse = getVirtualFunc<uintptr_t>(reinterpret_cast<void *>(client), 16);
	input = **reinterpret_cast<CInput***>(getAbsoluteAddress(activateMouse, 3, 7));
    Log::log(LOG, " Input %lx", (uintptr_t)input);

    /* Get PlayerResource */
	uintptr_t instructionAddr = PatternScan::findFirstInModule("/client_client.so", " 48 8B 05 ? ? ? ? 55 48 89 E5 48 85 C0 74 10 48");
	_playerResource = reinterpret_cast<PlayerResource**>(getAbsoluteAddress(instructionAddr, 3, 7));
    Log::log(LOG, " PlayerResource %lx", (uintptr_t)_playerResource);

    renderBeams = **relativeToAbsolute<ViewRenderBeams***>(PatternScan::findFirstInModule("/client_client.so", "4C 89 F6 4C 8B 25 ? ? ? ? 48 8D 05") + 6); // Credit: danielkrupinski
    Log::log(LOG, " renderBeams %lx", (uintptr_t)renderBeams);
   
    Log::log(LOG, "Initialised interfaces!");
    Log::log(LOG, "Initialising varaibles...");

    // cl vars
    Vars::cl_bob_lower_amt = Interfaces::convar->FindVar("cl_bob_lower_amt");
    Log::log(LOG, " cl_bob_lower_amt %lx", (uintptr_t)Vars::cl_bob_lower_amt);

    Vars::cl_bobamt_lat = Interfaces::convar->FindVar("cl_bobamt_lat");
    Log::log(LOG, " cl_bobamt_lat %lx", (uintptr_t)Vars::cl_bobamt_lat);

    Vars::cl_bobamt_vert = Interfaces::convar->FindVar("cl_bobamt_vert");
    Log::log(LOG, " cl_bobamt_vert %lx", (uintptr_t)Vars::cl_bobamt_vert);

    Vars::cl_bobcycle = Interfaces::convar->FindVar("cl_bobcycle");
    Log::log(LOG, " cl_bobcycle %lx", (uintptr_t)Vars::cl_bobcycle);
    
    Vars::cl_csm_enabled = Interfaces::convar->FindVar("cl_csm_enabled");
    Log::log(LOG, " cl_csm_enabled %lx", (uintptr_t)Vars::cl_csm_enabled);

    Vars::cl_interp = Interfaces::convar->FindVar("cl_interp");
    Log::log(LOG, " cl_interp %lx", (uintptr_t)Vars::cl_interp);

    Vars::cl_interp_ratio = Interfaces::convar->FindVar("cl_interp_ratio");
    Log::log(LOG, " cl_interp_ratio %lx", (uintptr_t)Vars::cl_interp_ratio);

    Vars::cl_interpolate = Interfaces::convar->FindVar("cl_interpolate");
    Log::log(LOG, " cl_interpolate %lx", (uintptr_t)Vars::cl_interpolate);

    Vars::cl_lagcompensation = Interfaces::convar->FindVar("cl_lagcompensation");
    Log::log(LOG, " cl_lagcompensation %lx", (uintptr_t)Vars::cl_lagcompensation);

    Vars::cl_phys_timescale = Interfaces::convar->FindVar("cl_phys_timescale");
    Log::log(LOG, " cl_phys_timescale %lx", (uintptr_t)Vars::cl_phys_timescale);

    Vars::cl_ragdoll_gravity = Interfaces::convar->FindVar("cl_ragdoll_gravity");
    Log::log(LOG, " cl_ragdoll_gravity %lx", (uintptr_t)Vars::cl_ragdoll_gravity);

    Vars::cl_windspeed = Interfaces::convar->FindVar("cl_windspeed");
    Log::log(LOG, " cl_windspeef %lx", (uintptr_t)Vars::cl_windspeed);

    Vars::cl_viewmodel_shift_left_amt = Interfaces::convar->FindVar("cl_viewmodel_shift_left_amt");
    Log::log(LOG, " cl_viewmodel_shift_left_amt %lx", (uintptr_t)Vars::cl_viewmodel_shift_left_amt);

    Vars::cl_viewmodel_shift_right_amt = Interfaces::convar->FindVar("cl_viewmodel_shift_right_amt");
    Log::log(LOG, " cl_viewmodel_right_right_amt %lx", (uintptr_t)Vars::cl_viewmodel_shift_right_amt);

    // engine vars
    Vars::engine_no_focus_sleep = Interfaces::convar->FindVar("engine_no_focus_sleep");
    Log::log(LOG, " engine_no_focus_sleep %lx", (uintptr_t)Vars::engine_no_focus_sleep);

    // host vars
    Vars::host_timescale = Interfaces::convar->FindVar("host_timescale");
    Log::log(LOG, " host_timescale %lx", (uintptr_t)Vars::host_timescale);

    // mat vars
    Vars::mat_drawgray = Interfaces::convar->FindVar("mat_drawgray");
    Log::log(LOG, " mat_drawgray %lx", (uintptr_t)Vars::mat_drawgray);

    Vars::mat_fullbright = Interfaces::convar->FindVar("mat_fullbright");
    Log::log(LOG, " mat_fullbright %lx", (uintptr_t)Vars::mat_fullbright);

    Vars::mat_postprocess_enable = Interfaces::convar->FindVar("mat_postprocess_enable");
    Log::log(LOG, " mat_postprocess_enable %lx", (uintptr_t)Vars::mat_postprocess_enable);

    Vars::mat_showlowresimage = Interfaces::convar->FindVar("mat_showlowresimage");
    Log::log(LOG, " mat_showlowresimage %lx", (uintptr_t)Vars::mat_showlowresimage);

    // mp vars
    Vars::mp_teammates_are_enemies = Interfaces::convar->FindVar("mp_teammates_are_enemies");
    Log::log(LOG, " mp_teammates_are_enemies  %lx", (uintptr_t)Vars::mp_teammates_are_enemies);

    // name vars
    Vars::name = Interfaces::convar->FindVar("name");
    Log::log(LOG, " name %lx", (uintptr_t)Vars::name);

    // net vars
    Vars::net_fakelag = Interfaces::convar->FindVar("net_fakelag");
    Log::log(LOG, " net_fakelag %lx", (uintptr_t)Vars::net_fakelag);

    // panorama vars
    Vars::panorama_disable_blur = Interfaces::convar->FindVar("@panorama_disable_blur");
    Log::log(LOG, " panorama_disable_blur %lx", (uintptr_t)Vars::panorama_disable_blur);

    // r vars
    Vars::r_3dsky = Interfaces::convar->FindVar("r_3dsky");
    Log::log(LOG, " r_3dsky %lx", (uintptr_t)Vars::r_3dsky);

    Vars::r_aspectratio = Interfaces::convar->FindVar("r_aspectratio");
    Log::log(LOG, " r_aspectratio %lx", (uintptr_t)Vars::r_aspectratio);

    Vars::r_drawmodelstatsoverlay = Interfaces::convar->FindVar("r_drawmodelstatsoverlay");
    Log::log(LOG, " r_drawmodelstatsoverlay %lx", (uintptr_t)Vars::r_drawmodelstatsoverlay);

    Vars::r_rainlength = Interfaces::convar->FindVar("r_rainlength");
    Log::log(LOG, " r_rainlength %lx", (uintptr_t)Vars::r_rainlength);

    Vars::r_rainspeed = Interfaces::convar->FindVar("r_rainspeed");
    Log::log(LOG, " r_rainspeed %lx", (uintptr_t)Vars::r_rainspeed);

    Vars::r_rainradius = Interfaces::convar->FindVar("r_rainradius");
    Log::log(LOG, " r_rainradius %lx", (uintptr_t)Vars::r_rainradius);

    Vars::r_rainwidth = Interfaces::convar->FindVar("r_rainwidth");
    Log::log(LOG, " r_rainwidth %lx", (uintptr_t)Vars::r_rainwidth);

    Vars::r_rainalpha = Interfaces::convar->FindVar("r_rainalpha");
    Log::log(LOG, " r_rainalpha %lx", (uintptr_t)Vars::r_rainalpha);

    Vars::r_RainSideVel = Interfaces::convar->FindVar("r_RainSideVel");
    Log::log(LOG, " r_RideSideVel %lx", (uintptr_t)Vars::r_RainSideVel);

    // sv vars
    Vars::sv_cheats = Interfaces::convar->FindVar("sv_cheats");
    Log::log(LOG, " sv_cheats %lx", (uintptr_t)Vars::sv_cheats);

    Vars::sv_gravity = Interfaces::convar->FindVar("sv_gravity");
    Log::log(LOG, " sv_gravity %lx", (uintptr_t)Vars::sv_gravity);

    Vars::sv_infinite_ammo = Interfaces::convar->FindVar("sv_infinite_ammo");
    Log::log(LOG, " sv_infinite_ammo %lx", (uintptr_t)Vars::sv_infinite_ammo);

    Vars::sv_showimpacts = Interfaces::convar->FindVar("sv_showimpacts");
    Log::log(LOG, " sv_showimpacts %lx", (uintptr_t)Vars::sv_showimpacts);

    // viewmodel vars
    Vars::viewmodel_fov = Interfaces::convar->FindVar("viewmodel_fov");
    Log::log(LOG, " viewmodel_fov %lx", (uintptr_t)Vars::viewmodel_fov);

    Vars::viewmodel_offset_x = Interfaces::convar->FindVar("viewmodel_offset_x");
    Log::log(LOG, " viewmodel_offset_x %lx", (uintptr_t)Vars::viewmodel_offset_x);

    Vars::viewmodel_offset_y = Interfaces::convar->FindVar("viewmodel_offset_y");
    Log::log(LOG, " viewmodel_offset_y %lx", (uintptr_t)Vars::viewmodel_offset_y);

    Vars::viewmodel_offset_z = Interfaces::convar->FindVar("viewmodel_offset_z");
    Log::log(LOG, " viewmodel_offset_z %lx", (uintptr_t)Vars::viewmodel_offset_z);

    Vars::weapon_recoil_scale = Interfaces::convar->FindVar("weapon_recoil_scale");
    Log::log(LOG, " weapon_recoil_scale %lx", (uintptr_t)Vars::weapon_recoil_scale);
    
    Log::log(LOG, "Initialised variables!");

    return true;
}

bool Interfaces::unload() {
    Log::log(LOG, "Unloading interfaces...");

    Log::log(LOG, "Unloaded interfaces!");
    return true;
}
