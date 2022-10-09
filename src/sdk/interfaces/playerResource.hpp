#pragma once
#include "../netvars.hpp"

#define TICKCOUNTWITHPING() (int)(Interfaces::globals->tickCount + ((float)playerResource->GetPing(Globals::localPlayer->index()) / 1000.0f) / Interfaces::globals->intervalPerTick)

class PlayerResource {
public:
	int GetPing(int index) {
		return *(int*)((uintptr_t)this + GETNETVAROFFSET("DT_PlayerResource", "m_iPing") + (index * 4));
	}
};
