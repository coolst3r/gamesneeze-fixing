#include "../../includes.hpp"
#include "features.hpp"

// Don't immediately shoot in deathmatch.
// TODO: Check if we're actually playing deathmatch.
inline bool allowMeToBuyInDeathmatch(Command *cmd) {
    if (!Globals::localPlayer->immune()) {
        return false;
    }

    cmd->buttons &= ~IN_ATTACK;

    return true;
}

// Ditto.
inline void noRecoil(Command *cmd) {
    auto recoil_scale = Vars::weapon_recoil_scale->GetFloat();

    cmd->viewAngle.x -= Features::NoVisualRecoil::aimPunch.x * recoil_scale;
    cmd->viewAngle.y -= Features::NoVisualRecoil::aimPunch.y * recoil_scale;
}

// Auto cock the revolver.
inline void autoCock(Command *cmd, Weapon *weapon) {
    if ((cmd->buttons & IN_ATTACK) != 0) {
        return;
    }

    cmd->buttons |= IN_ATTACK;
                
    float time = weapon->postponeFireReadyTime();

    if (time >= std::numeric_limits<float>::max()) {
        return;
    }

    if (time < Interfaces::globals->currentTime) {
        cmd->buttons &= ~IN_ATTACK;
    }
}

// Whether autowall should even attempt to shoot.
inline bool shouldShoot(Weapon *weapon, ItemIndex itemIndex) {
    if (Interfaces::globals->currentTime < weapon->nextAttack()) {
        return false;
    }

    if (weapon->isGrenade()) {
        return false;
    }

    if (itemIndex == ItemIndex::WEAPON_HEALTHSHOT) {
        return false;
    }
    
    return true;
}

// These weapons are not automatic, make them automatic.
// Don't use the currentTime <= nextAttack with automatic weapons, it's inefficient.
inline void simulateAutomatic(Command *cmd, Weapon *weapon, ItemIndex itemIndex) {
    switch (itemIndex) {
    case ItemIndex::WEAPON_TASER:
    case ItemIndex::WEAPON_HKP2000:
    case ItemIndex::WEAPON_AWP:
    case ItemIndex::WEAPON_DEAGLE:
    case ItemIndex::WEAPON_ELITE:
    case ItemIndex::WEAPON_GLOCK:
    case ItemIndex::WEAPON_NOVA:
    case ItemIndex::WEAPON_P250:
    case ItemIndex::WEAPON_SAWEDOFF:
    case ItemIndex::WEAPON_SG556:
    case ItemIndex::WEAPON_TEC9:
    case ItemIndex::WEAPON_USP_SILENCER: {
        if ((cmd->buttons & IN_ATTACK) != 0) {
            if (Interfaces::globals->currentTime <= weapon->nextAttack()) {
                cmd->buttons &= ~IN_ATTACK;
            }
        }

        break;
    }
    default: {

    } }
}

// You don't reload if you stay in attack.
inline bool reloadFix(Command *cmd, Weapon *weapon) {
    if (weapon->ammo() != 0) {
        return false;
    }

    cmd->buttons &= ~IN_ATTACK;

    return true;
}

// Switching weapons while in attack stops firing.
// Most noticable in Arms Race.
inline void switchWeaponFix(Command *cmd, Weapon *weapon) {
    // https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/in_main.cpp#L1166
    if (cmd->weaponSelect == 0) {
        return;
    }

    cmd->buttons &= ~IN_ATTACK;
}

inline bool iteratePlayers(
    Command *cmd,
    Weapon *weapon,
    ItemIndex itemIndex,
    int hitgroups
) {
    if (!shouldShoot(weapon, itemIndex)) {
        return false;
    }

    // No hitgroups selected, skip the below loops.
    // TODO: implement them (only targets head rn)
    if (hitgroups == 0) {
        return false;
    }

    auto targetHitChance = static_cast<float>(CONFIGINT("Rage>RageBot>Default>Hit Chance"));
    auto minimumDamage = CONFIGINT("Rage>RageBot>Default>Minimum Damage");
    auto spreadLimit = static_cast<float>(CONFIGINT("Rage>RageBot>Default>Spread Limit"));

    for (int i = 1; i < Interfaces::globals->maxClients; ++i) {
        auto player = Interfaces::entityList->player(i);

        if (player == nullptr) {
            continue;
        }

        if (!player->hittable()) {
            continue;
        }

        if (itemIndex == ItemIndex::WEAPON_KNIFE) {
            auto localOrigin = Globals::localPlayer->origin();
            auto playerOrigin = player->origin();
            auto distance = getDistance(localOrigin, playerOrigin);

            if (distance < 32) {
                cmd->buttons |= IN_ATTACK;

                return true;
            }

            continue;
        }

        Features::Defuse::canDefuse = true;

        Matrix3x4 boneMatrix[128];

        if (!player->getAnythingBones(boneMatrix)) {
            continue;
        }

        auto model = Interfaces::modelInfo->GetStudioModel(player->model());

        if (model == nullptr) {
            continue;
        }

        auto eyePos = Globals::localPlayer->eyePos();
        auto best_damage = player->best_damage();

        if (best_damage.damage == -1.0f || best_damage.bone == -1) {
            continue;
        }

        auto bonePos = player->getBonePos(best_damage.bone);
        cmd->viewAngle = calcAngle(eyePos, bonePos);
        normalizeAngle(cmd->viewAngle);

        Features::RageBot::spreadRadians = weapon->inaccuracy() + weapon->spread();
        Features::RageBot::spread = sdk::to_degrees(Features::RageBot::spreadRadians);
        Features::RageBot::range = weapon->info()->range();
        Features::RageBot::rangeModifier = weapon->info()->rangeModifier();
        Features::RageBot::distance = getDistance(eyePos, bonePos);
        Features::RageBot::distanceDelta = std::clamp(((Features::RageBot::range - Features::RageBot::distance) / Features::RageBot::range) - Features::RageBot::rangeModifier, 0.0f, 1.0f);
        Features::RageBot::hitChance = 100.0f - ((1.0 - Features::RageBot::distanceDelta) * Features::RageBot::spread);

        if (Features::RageBot::spread > spreadLimit) {
            continue;
        }

        if (Features::RageBot::hitChance < targetHitChance) {
            continue;
        }

        if (best_damage.damage < minimumDamage) {
            continue;
        }
      
        cmd->buttons |= IN_ATTACK;
        cmd->tickCount = player->tickbase();

        return true;
    }

    return false;
}

bool Features::RageBot::createMove(Command *cmd) {
    if (global::weapon == nullptr) {
        cmd->buttons &= ~IN_ATTACK;

        return false;
    }

    auto weapon = global::weapon;
    auto itemIndex = weapon->itemIndex();
    auto hitboxes = CONFIGINT("Rage>RageBot>Default>Hitboxes") & 0x1F;
    auto angles = cmd->viewAngle;
    auto wantAttack = (cmd->buttons & IN_ATTACK) != 0;

    if (itemIndex == ItemIndex::WEAPON_KNIFE) {
        return wantAttack;
    }

    if (iteratePlayers(cmd, weapon, itemIndex, hitboxes)) {
        wantAttack = true;
    } else {
        cmd->viewAngle = angles;
    }

    noRecoil(cmd);

    if (allowMeToBuyInDeathmatch(cmd)) {
        return false;
    }

    if (reloadFix(cmd, weapon)) {
        return false;
    }

    if (itemIndex == WEAPON_REVOLVER) {
        autoCock(cmd, weapon);
    } else {
        simulateAutomatic(cmd, weapon, itemIndex);
    }

    switchWeaponFix(cmd, weapon);

    return wantAttack;
}
