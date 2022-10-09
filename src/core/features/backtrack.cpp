#include "../../includes.hpp"
#include "features.hpp"
#include <cfloat>
#include <cstring>

void Features::Backtrack::store(Command *cmd) {
    if (!CONFIGBOOL("Legit>Backtrack>Backtrack")) {
        return;
    }

    if (!Interfaces::engine->IsInGame()) {
        return;
    }

    if (Globals::localPlayer == nullptr) {
        return;
    }

    // Store
    BackTrackTick currentTick;

    for (int i = 1; i < Interfaces::globals->maxClients; ++i) {
        auto player = Interfaces::entityList->player(i);

        if (player == nullptr) {
            continue;
        }

        if (!player->hittable()) {
            if (currentTick.players.find(i) != currentTick.players.end()) {
                currentTick.players.erase(i);
            }

            continue;
        }

        BacktrackPlayer backtrack;

        backtrack.playerIndex = i;
        backtrack.playerFlags = player->flags();
        backtrack.playerVelocity = player->velocity().Length2D();

        if (player->getAnythingBones(backtrack.boneMatrix)) {
            currentTick.players.insert(std::pair<int, BacktrackPlayer>(
                i,
                backtrack
            ));
        }
            
        backtrack.playerHeadPos = Vector{
            backtrack.boneMatrix[8][0][3],
            backtrack.boneMatrix[8][1][3],
            backtrack.boneMatrix[8][2][3]
        };
    }

    currentTick.tickCount = cmd->tickCount;
    backtrackTicks.insert(backtrackTicks.begin(), currentTick);

    // Delete ticks we cant backtrack
    while (static_cast<int>(backtrackTicks.size()) > CONFIGINT("Legit>Backtrack>Backtrack Ticks")) {
        backtrackTicks.pop_back();
    }
}

void Features::Backtrack::createMove(Command *cmd) {
    if (!CONFIGBOOL("Legit>Backtrack>Backtrack")) {
        return;
    }

    if (cmd->tickCount == 0) {
        return;
    }

    if (!Interfaces::engine->IsInGame()) {
        return;
    }

    if (Globals::localPlayer == nullptr) {
        return;
    }

    // Find how far we should backtrack in this tick
    auto viewAngle = Interfaces::engine->viewAngle();
    viewAngle += Globals::localPlayer->aimPunch() * 2;

    float closestDelta = FLT_MAX;
    int closestTick = cmd->tickCount;

    if (cmd->buttons & IN_ATTACK) {
        for (BackTrackTick tick : backtrackTicks) {
            for (auto player : tick.players) {
                auto p = Interfaces::entityList->player(player.second.playerIndex);
                
                if (p) {
                    if (p->health() > 0 && !p->dormant()) {
                        Vector localPlayerEyePos = Globals::localPlayer->eyePos();

                            Vector targetEyePos = Vector(player.second.boneMatrix[8][0][3], player.second.boneMatrix[8][1][3], player.second.boneMatrix[8][2][3]); // 8 is headbone in bonematrix

                            QAngle angleToCurrentPlayer = calcAngle(localPlayerEyePos, targetEyePos);
                            angleToCurrentPlayer -= viewAngle;
                            if (angleToCurrentPlayer.y > 180.f) {
                                angleToCurrentPlayer.y -= 360.f;
                            }

                            if (angleToCurrentPlayer.Length() < closestDelta) {
                                closestDelta = angleToCurrentPlayer.Length();
                                closestTick = tick.tickCount;
                            }
                        }
                        else {
                            // If p is dormant or dead then erase player from the tick
                            tick.players.erase(player.first);
                        }
                    }
                    else {
                        // If p doesnt exist then erase player from the tick
                        tick.players.erase(player.first);
                    }
                }
            }
        }

    lastBacktrack = cmd->tickCount - closestTick; // To show how much you backtracked in hitlogs
    cmd->tickCount = closestTick;
}
