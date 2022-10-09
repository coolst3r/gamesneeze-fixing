#include "features.hpp"
#include <vector>

Entity *findPlayerThatRayHits(Vector start, Vector end, Trace *traceToPlayer) {
    Ray ray = Ray{start, end};
    TraceFilter filter;
    filter.pSkip = Globals::localPlayer;

    Interfaces::trace->TraceRay(ray, CONTENTS_HITBOX | CONTENTS_MONSTER | CONTENTS_SOLID, &filter, traceToPlayer);

    return traceToPlayer->m_pEntityHit;
}

void Features::Triggerbot::createMove(Command *cmd) {
    if (Globals::localPlayer == nullptr) {
        return;
    }

    if (!(CONFIGBOOL("Legit>Triggerbot>Triggerbot") && Menu::CustomWidgets::isKeyDown(CONFIGINT("Legit>Triggerbot>Key")))) {
        return;
    }

    auto weapon = Interfaces::entityList->activeWeapon(Globals::localPlayer);

    if (weapon == nullptr) {
        return;
    }

    auto viewAngles = cmd->viewAngle;

    viewAngles.x += Features::NoVisualRecoil::aimPunch.x * 2.0f;
    viewAngles.y += Features::NoVisualRecoil::aimPunch.y * 2.0f;            

    auto endPos = Vector{0, 0, 0};
    Trace traceToPlayer;
    auto headHitchance = 0;
    auto bodyHitchance = 0;
    auto spread = sdk::to_degrees(weapon->inaccuracy() + weapon->spread());

    for (int i = 0; i < 100; ++i) {
        auto randomSpreadAngle = QAngle{
            randFloat(0, spread) - (spread / 2),
            randFloat(0, spread) - (spread / 2),
            randFloat(0, spread) - (spread / 2)
        };
                
        angleVectors(viewAngles + randomSpreadAngle, endPos);
                
        endPos = Globals::localPlayer->eyePos() + (endPos * 4096);

        auto entity = findPlayerThatRayHits(Globals::localPlayer->eyePos(), endPos, &traceToPlayer);

        if (entity == nullptr) {
            continue;
        }

        if (entity->clientClass()->m_ClassID != CCSPlayer) {
            continue;
        }

        auto player = reinterpret_cast<Player *>(entity);

        if (!player->hittable() && player->visible()) {
            return;
        }

        switch (traceToPlayer.hitgroup) {
        case HITGROUP_HEAD:
            headHitchance++;
        break;
        case HITGROUP_CHEST:
        case HITGROUP_STOMACH:
            bodyHitchance++;
            break;
        default:
            break;
        }
            
        static bool shotLastTick = false;

        if (CONFIGINT("Legit>Triggerbot>Head Hitchance") && headHitchance >= CONFIGINT("Legit>Triggerbot>Head Hitchance") && !shotLastTick) {
            cmd->buttons |= IN_ATTACK;
            shotLastTick = true;
        } else if (CONFIGINT("Legit>Triggerbot>Body Hitchance") && bodyHitchance >= CONFIGINT("Legit>Triggerbot>Body Hitchance") && !shotLastTick) {
            cmd->buttons |= IN_ATTACK;
            shotLastTick = true;
        } else {
            shotLastTick = false;
        }
    }
}
