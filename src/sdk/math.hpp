#pragma once
#include "sdk.hpp"

inline QAngle originalAngle{};
inline float originalForwardMove = 0.0f;
inline float originalSideMove = 0.0f;

inline void startMovementFix(Command* cmd) {
    originalAngle = cmd->viewAngle;
    originalForwardMove = cmd->move.x;
    originalSideMove = cmd->move.y;
}

inline void endMovementFix(Command* cmd) {
    // this was just taken from designer bc im lazy
    // https://github.com/designer1337/csgo-cheat-base/blob/09fa2ba8de52eef482bbc82f682976e369191077/dependencies/math/math.cpp#L4
    float deltaViewAngle;
	float f1;
	float f2;

	if (originalAngle.y < 0.f)
		f1 = 360.0f + originalAngle.y;
	else
		f1 = originalAngle.y;

	if (cmd->viewAngle.y < 0.0f)
		f2 = 360.0f + cmd->viewAngle.y;
	else
		f2 = cmd->viewAngle.y;

	if (f2 < f1)
		deltaViewAngle = abs(f2 - f1);
	else
		deltaViewAngle = 360.0f - abs(f1 - f2);

	deltaViewAngle = 360.0f - deltaViewAngle;

	cmd->move.x = cos(
		sdk::to_radians(deltaViewAngle))
		* originalForwardMove
		+ cos(sdk::to_radians(deltaViewAngle + 90.f))
		* originalSideMove;
	
	cmd->move.y = sin(
		sdk::to_radians(deltaViewAngle))
		* originalForwardMove
		+ sin(sdk::to_radians(deltaViewAngle + 90.f))
		* originalSideMove;
    // TODO: support upmove
}

inline void normalizePitch(float& pitch) {
	pitch = std::clamp(pitch, -89.0f, 89.0f);
}

inline void normalizeYaw(float& yaw) {
	while (yaw > 180.0f) {
		yaw -= 360.0f;
	}

	while (yaw < -180.0f) {
		yaw += 360.0f;
	}
}

inline void normalizeAngle(QAngle& angle) {
	normalizePitch(angle.x);
	normalizeYaw(angle.y);
	angle.z = 0.0f;
}

inline void clampAngle(QAngle& angle) {
	angle.x = std::clamp(angle.x, -89.0f, 89.0f);
	angle.y = std::clamp(angle.y, -180.0f, 180.0f);
	angle.z = 0.0f;
}

inline QAngle calcAngle(const Vector& src, const Vector& dst) {
	QAngle vAngle;
	Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
	double hyp = sqrt(delta.x*delta.x + delta.y*delta.y);

	vAngle.x = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
	vAngle.y = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
	vAngle.z = 0.0f;

	if (delta.x >= 0.0)
		vAngle.y += 180.0f;

	return vAngle;
}

inline void angleVectors(const QAngle &angles, Vector& forward) {
	forward.x = cos(sdk::to_radians(angles.x)) * cos(sdk::to_radians(angles.y));
	forward.y = cos(sdk::to_radians(angles.x)) * sin(sdk::to_radians(angles.y));
	forward.z = -sin(sdk::to_radians(angles.x));
}


inline void vectorAngle(const Vector& vec, QAngle& angles) {
    float pitch, yaw;
    if (!(vec.x || vec.y)) {
        pitch = vec.z > 0 ? 270.f : 90.f;
        yaw = 0.f;
    } else {
        pitch = atan2f(-vec.z, vec.Length2D()) * 180.f / M_PI;
        if (pitch < 0.f) pitch += 360.f;
        yaw = atan2f(vec.y, vec.x) * 180.f / M_PI;
        if (yaw < 0.f) yaw += 360.f;
    }
    angles.x = pitch;
    angles.y = yaw;
    angles.z = 0.f;
}

inline float getDistance(Vector pos1, Vector pos2) {
    // Do 3d pythag
    float a = abs(pos1.x-pos2.x);
    float b = abs(pos1.y-pos2.y);
    float c = abs(pos1.z-pos2.z);
    return sqrt(pow(a, 2.f) + pow(b, 2.f) + pow(c, 2.f));
}

inline float getDistanceNoSqrt(Vector pos1, Vector pos2) {
    // When you dont need an exact distance and just want to see if 
	// something is x further than something else for example theres no need to sqrt it
    float a = abs(pos1.x-pos2.x);
    float b = abs(pos1.y-pos2.y);
    float c = abs(pos1.z-pos2.z);
    return pow(a, 2.f) + pow(b, 2.f) + pow(c, 2.f);
}

inline Vector2D directionFromYawAndVelocity(float yaw, Vector velocity) {
	auto cosYaw = std::cos(yaw / 180.0f * sdk::numeric_consts<float>::pi());
	auto sinYaw = std::cos(yaw / 180.0f * sdk::numeric_consts<float>::pi());

	return Vector2D {
		(velocity.x * cosYaw) + (velocity.y * sinYaw),
		(velocity.y * cosYaw) - (velocity.x * sinYaw),
	};
}

bool worldToScreen(const Vector& origin, Vector& screen);

inline void sanitizeAngle(QAngle& angle) {
    normalizeAngle(angle);
    angle.x = std::clamp(angle.x, -89.0f, 89.0f);
    angle.y = std::clamp(angle.y, -180.0f, 180.0f);
    angle.z = 0.0f;
}

inline float randomFloat(float div) {
	return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / div);
}
