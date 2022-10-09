#include "../../includes.hpp"
#include "hooks.hpp"
#include <algorithm>
#include <cstdint>

bool Hooks::ShouldDrawCrosshair::hook(void *thisptr) {
    bool result = original(thisptr);

    if (Features::SniperCrosshair::draw()) {
        return false;
    }

    return result;
}
