#include "features.hpp"
#include <cstdio>
// Hitmarkers are triggered in hooks/events.cpp

void Features::Hitmarkers::draw() {
    if (Interfaces::engine->IsInGame()) {
        if (CONFIGBOOL("Misc>Misc>Hitmarkers>Hitmarkers")) {
            if (Interfaces::globals->currentTime < drawHitmarkerTill) {
                ImColor col = ImColor(255, 255, 255, 255);
                col.Value.w = (Interfaces::globals->currentTime < drawHitmarkerTill-0.2) ? 1.f : 0 + ((drawHitmarkerTill-Interfaces::globals->currentTime)*6);
                Globals::drawList->AddLine(ImVec2((Globals::screenSizeX/2) - 8, (Globals::screenSizeY/2) - 8), ImVec2((Globals::screenSizeX/2) - 4, (Globals::screenSizeY/2) - 4), col);
                Globals::drawList->AddLine(ImVec2((Globals::screenSizeX/2) + 8, (Globals::screenSizeY/2) - 8), ImVec2((Globals::screenSizeX/2) + 4, (Globals::screenSizeY/2) - 4), col);
                Globals::drawList->AddLine(ImVec2((Globals::screenSizeX/2) - 8, (Globals::screenSizeY/2) + 8), ImVec2((Globals::screenSizeX/2) - 4, (Globals::screenSizeY/2) + 4), col);
                Globals::drawList->AddLine(ImVec2((Globals::screenSizeX/2) + 8, (Globals::screenSizeY/2) + 8), ImVec2((Globals::screenSizeX/2) + 4, (Globals::screenSizeY/2) + 4), col);
            }
            else if (Interfaces::globals->currentTime-10 < drawHitmarkerTill) {
                drawHitmarkerTill = 0; // If its more than 10 secs in future, its probably too far to keep drawing and something has gone wrong, so set it to 0
            }
        }

        if (CONFIGBOOL("Misc>Misc>Hitmarkers>Damage Markers")) {
            for (DamageMarker damageMarker : damageMarkers) {
                Vector pos;
                if (worldToScreen(damageMarker.position, pos)) {
                    if (Interfaces::globals->currentTime < damageMarker.drawHitmarkerTill) {
                        char damage[32];
                        snprintf(damage, sizeof(damage), "%i", damageMarker.damage);

                        ImColor col = ImColor(0, 0, 0, 255);
                        col.Value.w = (Interfaces::globals->currentTime < damageMarker.drawHitmarkerTill-0.2) ? 1.f : 0 + ((damageMarker.drawHitmarkerTill-Interfaces::globals->currentTime)*6);

                        Globals::drawList->AddText(ImVec2(pos.x+1, pos.y+1), col, damage); // Black shadow

                        col.Value.x = 1;
                        col.Value.y = damageMarker.headshot ? 0.19f : 1;
                        col.Value.z = damageMarker.headshot ? 0.19f : 1;

                        Globals::drawList->AddText(ImVec2(pos.x, pos.y), col, damage);
                    }
                    else {
                        damageMarkers.erase(damageMarkers.begin());
                    }
                }
            }
        }
    }
}
