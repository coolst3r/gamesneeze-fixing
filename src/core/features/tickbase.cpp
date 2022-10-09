#include "../../includes.hpp"
#include "features.hpp"

inline int ticksAllowedForProcessing = 0;

bool canShift(int ticks, bool shiftAnyway = false) {
    if (Globals::localPlayer == nullptr) {
        return false;
    }

    if (Globals::localPlayer->health() == 0) {
        return false;
    }

    if (ticks <= 0) {
        return false;
    }

    if (shiftAnyway) {
        return true;
    }

    if ((ticksAllowedForProcessing - ticks) < 0) {
        return false;
    }

    auto weapon = Interfaces::entityList->activeWeapon(Globals::localPlayer);

    if (weapon) {
        return false;
    }

    auto nextAttack = weapon->nextAttack() + (ticks * Interfaces::globals->intervalPerTick);

    //if (nextAttack)
}
