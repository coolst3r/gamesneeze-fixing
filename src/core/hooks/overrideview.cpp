#include "../../includes.hpp"
#include "hooks.hpp"
#include "../features/features.hpp"

void Hooks::OverrideView::hook(void *self, ViewSetup *view) {
    auto in_third = CONFIGBOOL("Visuals>World>World>Third Person");

    // todo: remove QAngle
    //       rename Vector to Vec3
    view->angles.x = global::view_angle.x;
    view->angles.y = global::view_angle.y;
    view->angles.z = global::view_angle.z;

    Interfaces::input->thirdperson = in_third;

    if (Interfaces::input->thirdperson) {
        auto camera_offset = Vector{
            cos(sdk::to_radians(global::view_angle.y)) * 100,
            sin(sdk::to_radians(global::view_angle.y)) * 100,
            sin(sdk::to_radians(-global::view_angle.x)) * 100
        };

        // todo: rename Trace to Summary
        Trace trace;
        Ray{global::eye_pos, (global::eye_pos - camera_offset)}.ignoredTrace(CONTENTS_SOLID, global::local, &trace);

        Interfaces::input->offset = Vector{
            global::view_angle.x,
            global::view_angle.y,
            100 * std::min(trace.fraction, 1.0f)
        };
    }

    Globals::FOV = view->fov;

    original(self, view);
}
