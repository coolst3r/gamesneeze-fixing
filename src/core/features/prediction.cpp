#include "features.hpp"
// Credit: AimTux/Fuzion

float m_flOldCurtime;
float m_flOldFrametime;

void Features::Prediction::start(Command* cmd) {
    inPrediction = true;

    if (Globals::localPlayer) {
        *Offsets::predictionSeed = rand() & 0x7FFFFFFF;

        m_flOldCurtime = Interfaces::globals->currentTime;
        m_flOldFrametime = Interfaces::globals->frameTime;

        Interfaces::globals->currentTime = Globals::localPlayer->tickbase() * Interfaces::globals->intervalPerTick;
        Interfaces::globals->frameTime = Interfaces::globals->intervalPerTick;

        Interfaces::movement->StartTrackPredictionErrors(Globals::localPlayer);

        Interfaces::moveHelper->SetHost(Globals::localPlayer);
        Interfaces::prediction->SetupMove(Globals::localPlayer, cmd, Interfaces::moveHelper, Offsets::moveData);
        Interfaces::movement->ProcessMovement(Globals::localPlayer, Offsets::moveData);
        Interfaces::prediction->FinishMove(Globals::localPlayer, cmd, Offsets::moveData);
    }
}

void Features::Prediction::end() {
    if (Globals::localPlayer) {
        Interfaces::movement->FinishTrackPredictionErrors(Globals::localPlayer);
        Interfaces::moveHelper->SetHost(0);

        *Offsets::predictionSeed = -1;

        Interfaces::globals->currentTime = m_flOldCurtime;
        Interfaces::globals->frameTime = m_flOldFrametime;
    }

    inPrediction = false;
}

void Features::Prediction::restoreEntityToPredictedFrame(int predicted_frame) {
    Offsets::restoreEntityToPredictedFrame(Interfaces::prediction, 0, predicted_frame);
}