#include "../../includes.hpp"
#include "entity.hpp"
#include "vector.h"
#include <vector>

constexpr std::array<int, 7> T_BONES = {
    /* head */ 8,
    /* elbow */ 40, 12,
    /* feet */ 75, 77, 68, 69,
};

constexpr std::array<int, 7> CT_BONES = {
    /* head */ 8,
    /* elbow */ 40, 12,
    /* feet */ 83, 85, 74, 75
};

struct Shot {
    float damage = -1.0f;
    int bone = -1;
};

struct Shots {
    std::vector<Shot> shots;
};

struct PlayerCache {
    bool visible = false;
    float damage = 0.0f;

    Shots shots;
    Shots standing_shots;

    Matrix3x4 boneMatrixHitbox[128];
    Matrix3x4 boneMatrixAnything[128];

    Vector get_pos(int pos) noexcept {
        return Vector{
            boneMatrixAnything[pos][0][3],
            boneMatrixAnything[pos][1][3],
            boneMatrixAnything[pos][2][3]
        };
    }
};

bool Player::enemy() {
    if (strstr(Offsets::getLocalClient(-1)->m_szLevelNameShort, "dz_")) {
        return (Globals::localPlayer->survivalTeam() == -1) ? true : (Globals::localPlayer->survivalTeam() != this->survivalTeam());
    }
    
    if (Vars::mp_teammates_are_enemies->GetInt() == 1) {
        return this != Globals::localPlayer;
    }

    return this->team() != Globals::localPlayer->team();

}

bool Player::hittable() {
    if (!this->enemy()) {
        return false;
    }

    // ignore immunity in deathmatch, arms race, etc
    if (this->immune()) {
        return false;
    }

    // ignore non-networked players (server stopped sending their data)
    if (this->dormant()) {
        return false;
    }

    // ignore dead players
    if (this->health() == 0) {
        return false;
    }

    return true;
}

bool visCheck(Player* player) {
    Matrix3x4 boneMatrix[128];

    if (player->getAnythingBones(boneMatrix)) {
        TraceFilter filter;
        filter.pSkip = Globals::localPlayer;


        Trace traceToHead;
        Ray rayToHead = Ray{Globals::localPlayer->eyePos(), player->getBonePos(8)};

                                    // solid|opaque|moveable|ignore nodraw
        Interfaces::trace->TraceRay(rayToHead, (0x1 | 0x80 | 0x4000 | 0x2000), &filter, &traceToHead);

        Trace traceToUpperSpinal;
        Ray rayToUpperSpinal = Ray{Globals::localPlayer->eyePos(), player->getBonePos(6)};

                                    // solid|opaque|moveable|ignore nodraw
        Interfaces::trace->TraceRay(rayToUpperSpinal, (0x1 | 0x80 | 0x4000 | 0x2000), &filter, &traceToUpperSpinal);

        return (traceToHead.m_pEntityHit == player) && (traceToUpperSpinal.m_pEntityHit == player) && !Offsets::lineGoesThroughSmoke(Globals::localPlayer->eyePos(), player->eyePos(), 1);
    }

    return false;
}

inline float damageMultiplier(HitGroups group) {
    switch (group) {
    case HitGroups::HITGROUP_HEAD:
        return 4.0f;
    case HitGroups::HITGROUP_STOMACH:
        return 1.25f;
    case HitGroups::HITGROUP_LEFTLEG:
    case HitGroups::HITGROUP_RIGHTLEG:
        return 0.75f;
    default:
        return 1.0f;
    }
}

inline void scaleDamage(HitGroups group, Player *enemy, float weaponArmorRatio, float& currentDamage) {
    currentDamage *= damageMultiplier(group);

    if (enemy->armor() > 0) {
        if (!(group == HitGroups::HITGROUP_HEAD && !enemy->helmet())) {
            currentDamage *= weaponArmorRatio * 0.5f;
        }
    }
}

constexpr float STEP = 4.0f;

inline bool traceToExit(
    Vector startPos,
    Vector direction,
    Trace *enterTrace,
    Trace *exitTrace,
    Vector &endPos
) {
    auto distance = 0.0f;

    while (distance <= 90.0f) {
        distance += STEP;
        endPos = startPos + direction * distance;

        // returns `0` or `1` if it has a mask apparently.
        auto contents = Interfaces::trace->getPointContents(endPos, MASK_SHOT_HULL | CONTENTS_HITBOX);

        if ((contents & MASK_SHOT_HULL) != 0 && (contents & CONTENTS_HITBOX) == 0) {
            continue;
        }

        auto newEndPos = endPos - (direction * STEP);

        Ray{endPos, newEndPos}.ignoredTrace(MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, exitTrace);

        if (exitTrace->startsolid && (exitTrace->surface.flags & SURF_HITBOX) != 0) {
            Ray{endPos, startPos}.ignoredTrace(MASK_SHOT_HULL | CONTENTS_HITBOX, exitTrace->m_pEntityHit, exitTrace);

            if ((exitTrace->fraction <= 1.0f || exitTrace->allsolid) && !exitTrace->startsolid) {
                endPos = exitTrace->endpos;

                return true;
            }

            continue;
        }

        if (!(exitTrace->fraction <= 1.0f || exitTrace->allsolid || exitTrace->startsolid) || exitTrace->startsolid) {
            if (exitTrace->m_pEntityHit != nullptr
             && enterTrace->m_pEntityHit->hittable()) {
                Features::RageBot::target = enterTrace->m_pEntityHit;

                return true;
            }

            continue;
        }

        if ((exitTrace->surface.flags & SURF_NODRAW) != 0) {
            continue;
        }
        
        if (exitTrace->plane.normal.Dot(direction) <= 1.0f) {
            auto fraction = exitTrace->fraction * STEP;

            endPos = endPos - (direction * fraction);

            return true;
        }
    }

    return false;
}

class ShotData {
public:
    Vector source;
    Trace enterTrace;
    Vector direction;
    TraceFilter filter;
    float traceLength;
    float traceLengthRemaining;
    float currentDamage;
    int penetrateCount;
};

inline bool bulletPenetration(WeaponInfo *weaponInfo, ShotData& shotData) {
    auto enterSurfaceData = Interfaces::physics->surfaceData(shotData.enterTrace.surface.surfaceProps);
    auto enterMaterial = enterSurfaceData->game.material;
    auto enterPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;

    shotData.traceLength += shotData.traceLengthRemaining * shotData.enterTrace.fraction;
    shotData.currentDamage *= powf(weaponInfo->rangeModifier(), shotData.traceLength * 0.002f);

    if (shotData.traceLength > 3000.0f || enterPenetrationModifier < 0.1f) {
        shotData.penetrateCount = 0;
    }

    if (shotData.penetrateCount <= 0) {
        return false;
    }

    auto end = Vector{0, 0, 0};
    Trace traceExit;

    if (!traceToExit(
        /* startPos */ shotData.enterTrace.endpos,
        /* direction */ shotData.direction,
        /* enterTrace */ &shotData.enterTrace,
        /* exitTrace */ &traceExit,
        /* endPos */ end
    )) {
        return false;
    }

    auto exitSurfaceData = Interfaces::physics->surfaceData(traceExit.surface.surfaceProps);
    auto exitMaterial = exitSurfaceData->game.material;
    auto exitPenetrationModifier = exitSurfaceData->game.flPenetrationModifier;
    auto finalDamageModifier = 0.16f;
    auto combinedPenetrationModifier = 0.0f;

    if (((shotData.enterTrace.contents & CONTENTS_GRATE) != 0) || enterMaterial == 89 || enterMaterial == 71) {
        combinedPenetrationModifier = 3.0f;
        finalDamageModifier = 0.05f;
    } else {
        combinedPenetrationModifier = (enterPenetrationModifier + exitPenetrationModifier) * 0.5f;
    }

    if (enterMaterial == exitMaterial) {
        if (exitMaterial == 87 || exitMaterial == 85) {
            combinedPenetrationModifier = 3.0f;
        } else if (exitMaterial == 76) {
            combinedPenetrationModifier = 2.0f;
        }
    }

    float v34 = std::fmaxf(0.0f, 1.0f / combinedPenetrationModifier);
    float v35 = (shotData.currentDamage * finalDamageModifier) + v34 * 3.0f * std::fmaxf(0.0f, (3.0f / weaponInfo->penetration()) * 1.25f);
    float thickness = (traceExit.endpos - shotData.enterTrace.endpos).Length();

    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;

    float lostDamage = std::fmaxf(0.0f, v35 + thickness);

    if (lostDamage > shotData.currentDamage) {
        return false;
    }

    if (lostDamage >= 0.0f) {
        shotData.currentDamage -= lostDamage;
    }

    if (shotData.currentDamage < 1.0f) {
        return false;
    }

    shotData.source = traceExit.endpos;
    shotData.penetrateCount--;

    // cant shoot through this
    return true;
}

inline bool simulateShot(Weapon *weapon, ShotData& shotData) {
    auto info = weapon->info();

    shotData.penetrateCount = 4;
    shotData.traceLength = 0.0f;
    shotData.currentDamage = static_cast<float>(info->damage());

    while ((shotData.penetrateCount > 0) && (shotData.currentDamage >= 1.0f)) {
        shotData.traceLengthRemaining = info->range() - shotData.traceLength;

        auto end = shotData.source + shotData.direction * shotData.traceLengthRemaining;
        auto newEnd = end + shotData.direction * 40.0f;

        Ray{shotData.source, end}.ignoredTrace(MASK_SHOT, Globals::localPlayer, &shotData.enterTrace);
        Ray{shotData.source, newEnd}.filteredTrace(MASK_SHOT, &shotData.filter, &shotData.enterTrace);
        Ray{shotData.source, newEnd}.ignoredTrace(MASK_SHOT, Globals::localPlayer, &shotData.enterTrace);

        if (shotData.enterTrace.fraction == 1.0f) {
            break;
        }

        if (shotData.enterTrace.hitgroup <= 7 /* hitgroup rightleg */
         && shotData.enterTrace.hitgroup > 0 /* generic */) {
            shotData.traceLength += shotData.enterTrace.fraction * shotData.traceLengthRemaining;
            shotData.currentDamage *= std::pow(info->rangeModifier(), shotData.traceLength * 0.002f);

            auto player = shotData.enterTrace.m_pEntityHit;
        
            if (!player->hittable()) {
                return false;
            }

            Features::RageBot::target = player;

            scaleDamage(shotData.enterTrace.hitgroup, player, info->weaponArmorRatio(), shotData.currentDamage);

            return true;
        }

        if (!bulletPenetration(info, shotData)) {
            break;
        }
    }

    return false;
}

inline float getDamage(Vector source, const Vector& point) {
    auto destination = point;

    ShotData shotData;
    shotData.source = Globals::localPlayer->eyePos();
    shotData.filter.pSkip = Globals::localPlayer;

    auto angles = calcAngle(shotData.source, destination);

    angleVectors(angles, shotData.direction);

    shotData.direction = shotData.direction.Normalize();

    auto weapon = Interfaces::entityList->activeWeapon(Globals::localPlayer);

    if (weapon == nullptr) {
        return -1.0f;
    }

    if (simulateShot(weapon, shotData)) {
        return shotData.currentDamage;
    }

    return -1.0f;
}

std::map<int, PlayerCache> playerCache;

bool cachePlayer(int i, Player *player) {
    if (player == nullptr) {
        return false;
    }

    if (player == Globals::localPlayer) {
        return false;
    }

    if (player->health() == 0) {
        return false;
    }

    if (player->dormant()) {
        return false;
    }
    
    PlayerCache cached;

    if (!player->setupBones(cached.boneMatrixHitbox, 128, BONE_USED_BY_HITBOX, Interfaces::globals->currentTime)) {
        return false;
    }
    
    if (!player->setupBones(cached.boneMatrixAnything, 128, BONE_USED_BY_ANYTHING, Interfaces::globals->currentTime)) {
        return false;
    }

    auto eye_pos = Globals::localPlayer->eyePos();
    auto eye_standing_pos = Globals::localPlayer->eyePos();

    eye_standing_pos.z += 64.0f;
    
    if (player->team() == 2) {
        for (const auto& bone : T_BONES) {
            auto bone_pos = cached.get_pos(bone);
            auto damage = getDamage(eye_pos, bone_pos);

            if (damage == -1.0f) {
                continue;
            }

            cached.shots.shots.push_back(Shot {
                damage,
                bone
            });
        }
    } else {
        for (const auto& bone : CT_BONES) {
            auto bone_pos = cached.get_pos(bone);
            auto damage = getDamage(eye_pos, bone_pos);
    
            if (damage == -1.0f) {
                continue;
            }

            cached.shots.shots.push_back(Shot {
                damage,
                bone
            });
        }
    }

    cached.visible = visCheck(player);

    playerCache.insert_or_assign(i, cached);

    return true;
}

void cachePlayers() {
    if (Globals::localPlayer == nullptr) {
        return;
    }

    if (!Interfaces::engine->IsInGame()) {
        return;
    }

    auto origin = Globals::localPlayer->origin();
    auto highest = std::max(Interfaces::entityList->highestEntityIndex(), Interfaces::globals->maxClients);

    for (int i = 1; i < highest; ++i) {
        auto player = Interfaces::entityList->player(i);

        if (!cachePlayer(i, player)) {
            playerCache.erase(i);
        }

        auto entity = Interfaces::entityList->entity(i);

        if (entity == nullptr) {
            continue;
        }

        if (entity->dormant()) {
            continue;
        }
        
        for (auto& item : entityDistanceMap) {
            if (item.second == i) {
                entityDistanceMap.erase(item.first);
            }
        }

        // This line contains a really hacky way of ensuring the fog controller stays in the entity distance map 
        // as there are other entities that are also at 0, 0, 0 so we just move fog controller to -1, -1, -1
        entityDistanceMap.insert(std::pair<float, int>(
            getDistance(origin, entity->clientClass()->m_ClassID 
                == EClassIds::CFogController ?
                Vector(-1, -1, -1) :
                entity->origin()),
            i
        ));
    }
}

Vector Player::getBonePos(int bone) {
    // get cached bonematrix from setupbones we got in fsn
    if (playerCache.find(index()) != playerCache.end()) {
        return playerCache[index()].get_pos(bone);
    }

    return Vector{0, 0, 0};
}

bool Player::getHitboxBones(Matrix3x4 *boneMatrix) {
    // get cached bonematrix from setupbones we got in fsn
    if (playerCache.find(index()) != playerCache.end()) {
        memcpy(boneMatrix, playerCache[index()].boneMatrixHitbox, sizeof(Matrix3x4) * 128);
        return boneMatrix;
    }

    return false;
}

bool Player::getAnythingBones(Matrix3x4 *boneMatrix) {
    // get cached bonematrix from setupbones we got in fsn
    if (playerCache.find(index()) != playerCache.end()) {
        memcpy(boneMatrix, playerCache[index()].boneMatrixAnything, sizeof(Matrix3x4) * 128);
        return boneMatrix;
    }

    return false;
}

BestDamage Player::best_damage() {
    BestDamage damage;

    if (playerCache.find(index()) != playerCache.end()) {
        auto cached = playerCache[index()];
        
        auto best = std::max_element(cached.shots.shots.begin(), cached.shots.shots.end(), [](Shot& a, Shot& b) {
            return a.damage < b.damage;
        });

        auto best_standing = std::max_element(cached.standing_shots.shots.begin(), cached.standing_shots.shots.end(), [](Shot& a, Shot& b) {
            return a.damage < b.damage;
        });

        if (best != cached.shots.shots.end()) {
            damage.damage = best->damage;
            damage.bone = best->bone;
        }

        if (best_standing != cached.standing_shots.shots.end()) {
            damage.standing_damage = best_standing->damage;
            damage.standing_bone = best_standing->bone;
        }
    }

    return damage;
}

bool Player::visible() {
    if (playerCache.find(index()) != playerCache.end()) {
        return playerCache[index()].visible;
    }

    return false;
}
