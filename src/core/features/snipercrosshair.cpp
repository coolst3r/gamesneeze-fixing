#include "../../includes.hpp"
#include "features.hpp"

bool Features::SniperCrosshair::draw() {
    return false;

    if (!Interfaces::engine->IsInGame()) {
        return false;
    }

    if (Globals::localPlayer == nullptr) {
        return false;
    }

    if (Globals::localPlayer->health() == 0) {
        return false;
    }

    auto weapon = Interfaces::entityList->activeWeapon(Globals::localPlayer);

    if (weapon == nullptr) {
        return false;
    }

    if (!weapon->isSniper()) {
        return false;
    }

    auto info = weapon->info();

    if (!CONFIGBOOL("Visuals>World>World>No Sniper Crosshair")) {
	    info->setWeaponType(sdk::weapon_type::SNIPER);

        return false;
    }

    if (!Globals::localPlayer->scoped()) {
	    info->setWeaponType(sdk::weapon_type::SNIPER);

        return false;
    }

	info->setWeaponType(sdk::weapon_type::KNIFE);

    return true;
}
