#pragma once
#include "../classes/vector.h"

struct CInput {
	std::byte _pad0[16];
	bool is_track_ir_available;
	bool is_mouse_iniialized;
	bool is_mouse_active;
	std::byte _pad1[162];
	bool thirdperson;
	bool camera_moving_with_mouse;
	Vector offset;
};
