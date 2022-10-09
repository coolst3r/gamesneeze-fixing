#pragma once
#include "../../includes.hpp"
#include <vector>

namespace Features {
    namespace Prediction {
        inline bool inPrediction;
        void start(Command *cmd);
        void end();
        void restoreEntityToPredictedFrame(int predicted_frame);
    }
    namespace ESP {
        void draw();
    }
    namespace Chams {
        void drawModelExecute(void* thisptr, void* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, Matrix3x4 *pCustomBoneToWorld);
    }
    namespace Nightmode {
        void onTonemapController(TonemapController* tonemapController);
    }
    namespace RankReveal {
        void createMove(Command* cmd);
    }
    namespace RecoilCrosshair {
        void frameStageNotify(FrameStage frame);
        void draw();
    }
    namespace SkinChanger {
        void frameStageNotify(FrameStage frame);
    }
    namespace LegitBot {
        void createMove(Command* cmd);
    }
    namespace Backtrack {
        struct BacktrackPlayer {
            Matrix3x4 boneMatrix[128];
            int playerIndex;
            int playerFlags;
            float playerVelocity;
            Vector playerHeadPos;
        };

        struct BackTrackTick {
            std::map<int, BacktrackPlayer> players;
            int tickCount;
        };

        inline int lastBacktrack;
        inline std::vector<BackTrackTick> backtrackTicks;

        void store(Command* cmd);
        void createMove(Command* cmd);
    }
    namespace Forwardtrack {
        void createMove(Command* cmd);
    }
    namespace ColorModulation {
        void updateColorModulation();
    }
    namespace ClantagChanger {
        void frameStageNotify(FrameStage frame);
    }
    namespace SkyboxChanger {
        void frameStageNotify(FrameStage frame);
    }
    namespace Spectators {
        void draw();
    }
    namespace PlayerList {
        void draw();
    }
    namespace Notifications {
        void draw();
        void addNotification(ImColor color, const char* fmt, ...);
    }
    namespace FlappyBird {
        void draw();
    }
    namespace UseSpam {
        void createMove(Command* cmd);
    }
    namespace AutoAccept {
        void emitSound(const char* pSoundEntry);
    }
    namespace Radar {
        void espPlayerLoop(Player* p);
    }
    namespace FastDuck {
        void createMove(Command *cmd);
    }
    namespace Triggerbot {
        void createMove(Command* cmd);
    }
    namespace RageBot {
        inline Player *target = nullptr;

        inline float range = 0.0f;
        inline float rangeModifier = 0.0f;
        inline float spreadRadians = 0.0f;
        inline float spread = 0.0f;
        inline float distance = 0.0f;
        inline float distanceDelta = 0.0f;
        inline float hitChance = 0.0f;

        bool createMove(Command* cmd);
    }
    namespace AntiAim {
        inline Matrix3x4 fakeBones[128];
        inline bool fakeduck = false;
        inline int choked = 0;

        // if the client is sending a shot to the server
        inline bool shot_last_tick = false;

        void createMove(Command *cmd);

        inline QAngle thirdViewAngle = QAngle{0, 0, 0};
    }
    namespace Hitmarkers {
        struct DamageMarker {
            float drawHitmarkerTill;
            Vector position;
            bool headshot;
            int damage;
        };

        inline std::vector<DamageMarker> damageMarkers;
        inline float drawHitmarkerTill;
        void draw();
    }
    namespace SlowWalk {
        void createMove(Command* cmd);
    }
    namespace NoFlash {
        void frameStageNotify(FrameStage frame);
    }
    namespace NoVisualRecoil {
        inline QAngle aimPunch{};

        void frameStageNotify(FrameStage frame);
    }
    namespace BulletTracers {
        void event(IGameEvent* event);
    }
    namespace Movement {
        inline int flagsBackup = 0;
        inline bool shouldEdgebug = false;
        inline bool shouldDuckNext = false;
        inline Vector velBackup = Vector{0, 0, 0};
        inline Vector edgebugPos = Vector{0, 0, 0};
        inline QAngle anglesBackup{};

        void backupAngle(Command *cmd);
        void restoreAngle(Command *cmd);
        void autoStrafe(Command *cmd);
        void prePredCreateMove(Command* cmd);
        void postPredCreateMove(Command* cmd);
        void edgeBugPredictor(Command* cmd);
        void airstuckWalk(Command *cmd);
        void draw();
    }
    namespace SniperCrosshair {
        bool draw();
    }
    namespace Variables {
        void frameStageNotify(FrameStage frame);
    }
    namespace AutoBuy {
        void event(IGameEvent* event);
    }
    namespace Defuse {
        inline QAngle angles{};
        inline bool canDefuse = false;
        inline bool performDefuse = false;
        
        void createMoveStart(Command *cmd);
        void createMoveEnd(Command *cmd);
        void onBombRender(PlantedC4 *bomb);
    }
    namespace Resolver {
        void frameStageNotify(FrameStage frame);
    }
}
