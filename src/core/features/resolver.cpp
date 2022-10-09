#include "../../includes.hpp"
#include "features.hpp"

constexpr float M_360_65536 = 360.0f / 65536.0f;
constexpr float M_65536_360 = 65536.0f / 360.0f;

/// https://github.com/VSES/SourceEngine2007/blob/master/src_main/public/mathlib/mathlib.h#L498
inline float angleMod(float angle) noexcept {
    return M_360_65536
        * static_cast<float>(static_cast<std::int32_t>(angle * M_65536_360) & 65535);
}

/// https://github.com/VSES/SourceEngine2007/blob/master/src_main/mathlib/mathlib_base.cpp#L3327
inline float approachAngle(float target, float value, float speed) noexcept {
    target = angleMod(target);
    value = angleMod(value);

    float delta = target - value;

    if (speed < 0) {
        speed = -speed;
    }

    if (delta < -180) {
        delta += 360;
    } else if (delta > 180) {
        delta -= 360;
    }

    if (delta > speed) {
        value += speed;
    } else if (delta < -speed) {
        value -= speed;
    } else {
        value = target;
    }

    return value;
}

inline void resolve(Player *player) noexcept {
    auto layers = player->animLayers();
    auto state = player->animState();
    auto lby = player->lby();
    auto eyeYaw = player->eyePos().y;

    auto goal = 0.f;
    auto yawModifier = (((state->timeSinceStoppedMoving * -0.3f) - 0.2f) * state->speed) + 1.0f;
    auto minYawModifier = state->minYaw * yawModifier;
    auto maxYawModifier = state->maxYaw * yawModifier;
    auto goalFeetYaw = state->goalFeetYaw;
    auto eyeFeetDelta = state->yaw - goalFeetYaw;
    auto lastClientSideAnimationUpdateTimeDelta = fabs(state->lastAnimUpdateFrame - state->lastAnimUpdateTime);

    layers[12].m_flWeight = 0.0f;

    normalizeYaw(lby);
    normalizeYaw(eyeYaw);
    normalizeYaw(goalFeetYaw);

    if (state->duckProgress > 0.0f) {
        auto duckingSpeed = std::clamp(state->landingAnimationTimeLeftUntilDone, 0.0f, 1.0f);

        yawModifier = yawModifier + ((state->duckProgress * state->landingAnimationTimeLeftUntilDone) * (0.5f - yawModifier));
    }

    if (eyeFeetDelta <= maxYawModifier) {
        goalFeetYaw += minYawModifier;
    } else {
        // This is the infamous "Max Delta" that people rant about, which we cannot use the same methods as antiaim desync,
        // as our percieved state of the enemy players is incorrect!
        // That's how desync works in the first place, and has nothing to do with eyeangles!
        goalFeetYaw -= maxYawModifier;
    }

    if (state->speed < 0.4f) {
        goal = approachAngle(eyeYaw, goalFeetYaw, ((state->duckProgress * 20.0f) + 30.0f) * lastClientSideAnimationUpdateTimeDelta);
        goal = approachAngle(lby, goalFeetYaw, lastClientSideAnimationUpdateTimeDelta * 57.5f);
    } else {
        goal = approachAngle(lby, goalFeetYaw, lastClientSideAnimationUpdateTimeDelta * 23.0f);
    }

    state->goalFeetYaw = goal;
}

void Features::Resolver::frameStageNotify(FrameStage frame) {
    if (frame != FRAME_NET_UPDATE_END) {
        return;
    }
    
    if (!CONFIGBOOL("Rage>RageBot>Default>Resolver")) {
        return;
    }

    if (!Interfaces::engine->IsInGame()) {
        return;
    }

    if (Globals::localPlayer == nullptr) {
        return;
    }

    for (int i = 1; i < Interfaces::globals->maxClients; ++i) {
        auto player = Interfaces::entityList->player(i);

        if (player == nullptr) {
            continue;
        }

        if (player == Globals::localPlayer) {
            continue;
        }

        resolve(player);
    }
}
