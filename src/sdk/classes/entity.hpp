#pragma once
#include "../netvars.hpp"
#include "vector.h"
#include "../../utils/utils.hpp"
#include "../interfaces/ivmodelrender.hpp"
#include <array>
#include <functional>

inline std::map<float, int, std::greater<float>> entityDistanceMap; // Map containing entity distance
void cachePlayers();

class AnimLayer {
public:
    char pad_0000[24];
    int m_nOrder;
    int m_nSequence; // 0x1C
    float_t m_flPrevCycle;
    float_t m_flWeight;
    float_t m_flWeightDeltaRate;
    float_t m_flPlaybackRate;
    float_t m_flCycle;
    void *m_pOwner; // 0x38 // player's thisptr
    char pad_0038[8]; // 0x40
}; //Size: 0x0048

class ICollideable {
public:
	virtual void pad0();
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;
};

struct BestDamage {
    float damage = -1.0f;
    float standing_damage = -1.0f;
    int bone = -1;
    int standing_bone = -1;
    bool duck = false;
};

class Entity {
public:
	inline void *networkable() {
		return reinterpret_cast<void *>(uintptr_t(this) + 16);
	}

	inline void *renderable() {
		return reinterpret_cast<void *>(uintptr_t(this) + 0x8);
	}

	inline ClientClass *clientClass() {
		typedef ClientClass *(* Fn)(void *);
		return getVirtualFunc<Fn>(networkable(), 2)(networkable());
	}

	inline bool dormant() {
		typedef bool (* Fn)(void *);
		return getVirtualFunc<Fn>(networkable(), 9)(networkable());
	}

	inline int index() {
		typedef int (* Fn)(void *);
		return getVirtualFunc<Fn>(networkable(), 10)(networkable());
	}

	inline model_t *model() {
		typedef model_t *(* Fn)(void *);
		return getVirtualFunc<Fn>(renderable(), 8)(renderable());
	}

	inline bool setupBones(
		Matrix3x4 *pBoneMatrix,
		int nMaxBones,
		int nBoneMask,
		float flCurTime = 0.0f
	) {
		typedef bool (* Fn)(void *, Matrix3x4 *, int, int, float);
		return getVirtualFunc<Fn>(renderable(), 13)(renderable(), pBoneMatrix, nMaxBones, nBoneMask, flCurTime);
	}

	inline bool shouldDraw() {
		typedef bool (* Fn)(void *);
		return getVirtualFunc<Fn>(renderable(), 5)(renderable());
	}

	inline const Vector& origin() {
		typedef const Vector&(* Fn)(void *);
		return getVirtualFunc<Fn>(this, 12)(this);
	}

	inline bool isPlayer() {
		typedef bool (* Fn)(void *);
		return getVirtualFunc<Fn>(this, 157)(this);
	}

	NETVAR("DT_BaseEntity", "m_Collision", collideable, ICollideable);
	NETVAR("DT_BaseEntity", "m_iTeamNum", team, int);
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool);
};

class Weapon;

class Player : public Entity {
public:
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, Vector);
	NETVAR("DT_BasePlayer", "m_nTickBase", tickbase, unsigned int);
	NETVAR("DT_CSPlayer", "m_iAccount", money, int);
    NETVAR("DT_BasePlayer", "m_iHealth", health, int);
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int);
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aimPunch, QAngle);
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", viewPunch, QAngle);
	NETVAR("DT_CSPlayer", "m_hActiveWeapon", activeWeapon, void *);
	NETVAR("DT_CSPlayer", "m_hObserverTarget", observerTarget, Player*);
	NETVAR("DT_CSPlayer", "m_bHasDefuser", defuser, bool);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", viewOffset, Vector);
    NETVAR("DT_CSPlayer", "m_angEyeAngles[0]", eyeAngles, QAngle);
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lbyTarget, float);
	NETVAR("DT_CSPlayer", "m_bIsScoped", scoped, bool);
	NETVAR("DT_BasePlayer", "deadflag", deadflag, bool);
    NETVAR("DT_CSPlayer", "m_flFlashDuration", flashDuration, float);
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", maxFlashAlpha, float);
	NETVAR("DT_CSPlayer", "m_bHasHelmet", helmet, bool);
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int);
	NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survivalTeam, int);
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", immune, bool);
	NETVAR("CBasePlayer", "m_hViewModel[0]", viewModel, int);
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lby, float);

	inline bool ducking() noexcept {
		return (flags() & sdk::player_flags::DUCKING) != 0;
	}

	inline bool on_ground() noexcept {
		return (flags() & sdk::player_flags::ON_GROUND) != 0;
	}

	inline bool on_partial_ground() noexcept {
		return (flags() & sdk::player_flags::ON_PARTIAL_GROUND) != 0;
	}

	inline std::array<AnimLayer, 13> animLayers() {
		return *reinterpret_cast<std::array<AnimLayer, 13> *>(reinterpret_cast<uintptr_t>(this) + Offsets::animLayers);
	}

	inline AnimState *animState() {
		return *reinterpret_cast<AnimState **>(reinterpret_cast<uintptr_t>(this) + Offsets::animState);
	}

	inline QAngle *viewAngle() {
		return reinterpret_cast<QAngle *>(reinterpret_cast<uintptr_t>(deadflag_ptr()) + 0x4);
	}

	inline Vector eyePos() {
		return origin() + ((viewOffset().z > 0) ? viewOffset() : Vector(0, 0, (flags() & (1 << 1)) ? 46 : 64)); // For some reason some ents' viewoffset is all 0s, this is a hacky fix for it
	}

	inline int crosshair() {
		return *reinterpret_cast<int *>(reinterpret_cast<uintptr_t>(defuser_ptr()) + 0x7C);
	}

	inline int moveType() {
		return *reinterpret_cast<int *>(reinterpret_cast<uintptr_t>(this) + GETNETVAROFFSET("DT_BaseEntity", "m_nRenderMode") + 1);
	}

	bool enemy();
	bool hittable();
	bool getHitboxBones(Matrix3x4* boneMatrix);
	bool getAnythingBones(Matrix3x4* boneMatrix);

	void saveData(const char *context, int slot, int type) {
		Offsets::saveData(this, context, slot, type);
	}

	void restoreData (const char *context, int slot, int type) {
		Offsets::restoreData(this, context, slot, type);
	}

	void onPostRestoreData() {
		Offsets::onPostRestoreData(this);
	}

	Vector getBonePos(int bone);
	BestDamage best_damage();
	bool visible();
};

class Item : public Entity {
public:
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", rawItemIndex, int);

	inline ItemIndex itemIndex() {
		// What does this actually remove? Skin bits?
		// Unknown indices are sure returned without.
		return static_cast<ItemIndex>(this->rawItemIndex() & 0xFFF);
	}
};

class FileWeaponInfo {
public:
	FileWeaponInfo();

	// each game can override this to get whatever values it wants from the script.
	virtual void Parse(KeyValues *pKeyValuesData, const char *szWeaponName);

	bool bParsedScript;
	bool bLoadedHudElements;

	char szClassName[80];
	char szPrintName[80];

	char szViewModel[80];
	char szWorldModel[80];
	char szAmmo1[32];
	char szWorldDroppedModel[80];
	char szAnimationPrefix[16];
	int iSlot;
	int iPosition;
	int iMaxClip1;
	int iMaxClip2;
	int iDefaultClip1;
	int iDefaultClip2;
	int iWeight;
	int iRumbleEffect;
	bool bAutoSwitchTo;
	bool bAutoSwitchFrom;
	int iFlags;
	char szAmmo2[32];
	char szAIAddOn[80];

	// sound blocks
	char aShootSounds[18][80];

	int iAmmoType;
	int iAmmo2Type;
	bool m_bMeleeWeapon;

	// if the weapon was built right-handed (defaults to true)
	// helps cl_righthand make the decision about whether to flip the model or not
	bool m_bBuiltRightHanded;
	bool m_bAllowFlipping;

	// sprite data, read from the data file
	int iSpriteCount;
	void *iconActive;
	void *iconInactive;
	void *iconAmmo;
	void *iconAmmo2;
	void *iconCrosshair;
	void *iconAutoaim;
	void *iconZoomedCrosshair;
	void *iconZoomedAutoaim;
	void *iconSmall;
};

class WeaponInfo : public FileWeaponInfo {
public:
	inline uintptr_t asPtr() const noexcept {
		return reinterpret_cast<uintptr_t>(this);
	}

	inline const char *consoleName() {
		return *reinterpret_cast<const char **>(asPtr() + 0x8);
	}

	inline int clipSize() {
		return *reinterpret_cast<int *>(asPtr() + 0x20);
	}

	inline int type() const noexcept {
		return *reinterpret_cast<int *>(asPtr() + 0x140);
	}

	inline bool isKnife() const noexcept {
		return type() == sdk::weapon_type::KNIFE;
	}

	inline bool isPistol() const noexcept {
		return type() == sdk::weapon_type::PISTOL;
	}

	inline bool isSmg() const noexcept {
		return type() == sdk::weapon_type::SMG;
	}

	inline bool isRifle() const noexcept {
		return type() == sdk::weapon_type::RIFLE;
	}

	inline bool isShotgun() const noexcept {
		return type() == sdk::weapon_type::SHOTGUN;
	}

	inline bool isSniper() const noexcept {
		return type() == sdk::weapon_type::SNIPER;
	}

	inline bool isMachine() const noexcept {
		return type() == sdk::weapon_type::MACHINE;
	}

	inline bool isBomb() const noexcept {
		return type() == sdk::weapon_type::BOMB;
	}

	inline bool isGrenade() const noexcept {
		return type() == sdk::weapon_type::GRENADE;
	}

	inline void setWeaponType(int type) {
		*reinterpret_cast<int *>(asPtr() + 0x140) = type;
	}

	inline int damage() {
		return *reinterpret_cast<int *>(asPtr() + 0x16C);
	}

	inline float weaponArmorRatio() {
		return *reinterpret_cast<float *>(asPtr() + 0x174);
	}

	inline float penetration() {
		return *reinterpret_cast<float *>(asPtr() + 0x17C);
	}

	inline float range() {
		return *reinterpret_cast<float *>(asPtr() + 0x188);
	}

	inline float rangeModifier() {
		return *reinterpret_cast<float *>(asPtr() + 0x18C);
	}

	inline float maxPlayerSpeed() {
		return *reinterpret_cast<float *>(asPtr() + 0x1B8);
	}

	inline int zoomLevels() {
		// NOTE doesn't work correctly on some weapons.
		// NOTE replace with DT_WeaponCSBaseGun.m_zoomLevel?
		return *reinterpret_cast<int *>(asPtr() + 0x23C);
	}

	inline char *tracerEffect() {
		return *reinterpret_cast<char **>(asPtr() + 0x290);
	}

	inline int *tracerFrequency() {
		return reinterpret_cast<int *>(asPtr() + 0x298);
	}
};

class Weapon : public Item {
public:
	NETVAR("DT_BaseCombatWeapon", "m_hOwner", owner, int);
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", ammo, int);
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", nextAttack, float);
	NETVAR("DT_BaseCombatWeapon", "m_iItemIDHigh", itemIDHigh, int);
	NETVAR("DT_BaseCombatWeapon", "m_iAccountID", accountID, int);
	NETVAR("DT_BaseCombatWeapon", "m_nFallbackPaintKit", paintKit, int);
	NETVAR("DT_BaseCombatWeapon", "m_flFallbackWear", wear, float);
	NETVAR("DT_BaseCombatWeapon", "m_nFallbackStatTrak", statTrack, int);

	NETVAR("DT_WeaponCSBase", "m_fAccuracyPenalty", accuracyPenalty, float);
	NETVAR("DT_WeaponCSBase", "m_flPostponeFireReadyTime", postponeFireReadyTime, float);
	NETVAR("DT_WeaponCSBase", "m_bReloadVisuallyComplete", reloadVisuallyComplete, int);

	inline int subtype() {
		typedef int (* Fn)(void *);
		return getVirtualFunc<Fn>(this, 350)(this);
	}

	inline float spread() {
		typedef float (* Fn)(void *);
		return getVirtualFunc<Fn>(this, 521)(this);
	}

	inline float inaccuracy() {
		typedef float (*Fn)(void *);
		return getVirtualFunc<Fn>(this, 551)(this);
	}

	inline void drawCrosshair() {
		// NOTE always returns 1
		typedef void (* Fn)(void *);
		return getVirtualFunc<Fn>(this, 472)(this);
	}

	inline WeaponInfo *info() {
		typedef WeaponInfo *(* Fn)(void *);
		return getVirtualFunc<Fn>(this, 529)(this);
	}

	inline bool isKnife() noexcept {
		return info()->isKnife();
	}

	inline bool isPistol() noexcept {
		return info()->isPistol();
	}

	inline bool isSmg() noexcept {
		return info()->isSmg();
	}

	inline bool isRifle() noexcept {
		return info()->isRifle();
	}

	inline bool isShotgun() noexcept {
		return info()->isShotgun();
	}

	inline bool isSniper() noexcept {
		return info()->isSniper();
	}

	inline bool isMachine() noexcept {
		return info()->isMachine();
	}

	inline bool isBomb() noexcept {
		return info()->isBomb();
	}

	inline bool isGrenade() noexcept {
		return info()->isGrenade();
	}
};

class PlantedC4 : public Item {
public:
	NETVAR("DT_PlantedC4", "m_flC4Blow", time, float);
	NETVAR("DT_PlantedC4", "m_mBombDefused", defused, bool);
};

class TonemapController {
public:
	NETVAR("DT_EnvTonemapController", "m_bUseCustomAutoExposureMin", useExposureMin, bool);
	NETVAR("DT_EnvTonemapController", "m_bUseCustomAutoExposureMax", useExposureMax, bool);
	NETVAR("DT_EnvTonemapController", "m_bUseCustomBloomScale", useCustomBloomScale, bool);
	NETVAR("DT_EnvTonemapController", "m_flCustomAutoExposureMin", exposureMin, float);
	NETVAR("DT_EnvTonemapController", "m_flCustomAutoExposureMax", exposureMax, float);
	NETVAR("DT_EnvTonemapController", "m_flCustomBloomScale", customBloomScale, float);
};

class FogController {
public:
	NETVAR("DT_FogController", "m_fog.enable", enable, bool);
	NETVAR("DT_FogController", "m_fog.start", start, float);
	NETVAR("DT_FogController", "m_fog.end", end, float);
	NETVAR("DT_FogController", "m_fog.farz", farz, float);
	NETVAR("DT_FogController", "m_fog.maxdensity", maxDensity, float);
	NETVAR("DT_FogController", "m_fog.colorPrimary", colorPrimary, int);
};
