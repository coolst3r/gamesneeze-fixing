#pragma once

struct UserCmd;

struct GlobalVars {
    const float realTime;
    const int frameCount;
    const float absoluteFrameTime;
    const float absoluteFrameStartTimeStandard;
    float currentTime;
    float frameTime;
    const int maxClients;
    const int tickCount;
    const float intervalPerTick;
    const float interpolationAmount;
    const int simulationTicksThisFrame;
    const int networkProtocol;
    const void *saveData;
    const bool client;
    const bool remoteClient;

    /*float serverTime(UserCmd *cmd = nullptr) const noexcept {
        static int tick;
        static UserCmd *lastCmd;

        if (cmd) {
            if (Globals::localPlayer && (!lastCmd || lastCmd->hasbeenpredicted)) {
                tick = Globals::localPlayer->tickBase();
            } else {
                tick++;
            }

            lastCmd = cmd;
        }

        return tick * intervalPerTick;
    }*/
};
