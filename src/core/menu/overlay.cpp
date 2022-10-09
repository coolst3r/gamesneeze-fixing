#include "../../includes.hpp"
#include <cstring>
#include <unistd.h>
#include <pwd.h>

template<class ...Args>
void drawText(const char *fmt, ImVec2 pos, ImColor color, Args... args) {
    char buffer[256];

    sprintf(
        buffer,
        fmt,
        args...
    );

    auto spos = ImVec2{pos.x + 1, pos.y + 2};
    Globals::drawList->AddText(spos, ImColor{0, 0, 0, 255}, buffer);
    Globals::drawList->AddText(pos, color, buffer);
}

static float offset = 100.0;

template<class ...Args>
void drawTextO(const char *fmt, ImColor color, Args... args) {
    drawText(fmt, ImVec2{100.0, offset}, color, args...);

    offset += 20.0;
}

void Menu::drawOverlay(ImDrawList *drawList) {
    Globals::drawList = drawList;

    if (CONFIGBOOL("Misc>Misc>Misc>Disable Watermark")) {
        drawText(
            "gamesneeze | %.1f FPS | %i ms",
            ImVec2{2, 2},
            ImColor{255, 255, 255, 255},
            ImGui::GetIO().Framerate,
            (Interfaces::engine->IsInGame() && playerResource) ?
                playerResource->GetPing(Interfaces::engine->GetLocalPlayer()) :
                0
        );
    }

    auto x3 = static_cast<float>(Globals::screenSizeX / 2);
    auto y3 = static_cast<float>(Globals::screenSizeY / 2);
        
    drawText(
        "%s",
        ImVec2{x3 - 30, y3},
        ImColor{255, 255, 255, 255},
        render_anti_aim_state(global::anti_aim_state)
    );

    offset = 100.0;

    drawTextO(
        "ammo = %d",
        ImColor{255, 255, 255, 255},
        global::ammo
    );

    drawTextO(
        "health = %d",
        ImColor{255, 255, 255, 255},
        global::health
    );

    drawTextO(
        "tick_count = %d",
        ImColor{255, 255, 255, 255},
        global::tick_count
    );

    drawTextO(
        "choked = %d",
        ImColor{255, 255, 255, 255},
        global::choked
    );

    drawTextO(
        "send_packet = %s",
        ImColor{255, 255, 255, 255},
        global::send_packet ? *global::send_packet ? "true" : "false" : "false"
    );

    drawTextO(
        "side = %.2f",
        ImColor{255, 255, 255, 255},
        global::side
    );

    drawTextO(
        "use_shoot_view_angle = %.2f",
        ImColor{255, 255, 255, 255},
        global::use_shoot_view_angle
    );

    Features::ESP::draw();
    Features::RecoilCrosshair::draw();
    Features::Spectators::draw();
    Features::PlayerList::draw();
    Features::FlappyBird::draw();
    Features::Notifications::draw();
    Features::Hitmarkers::draw();
    Features::Movement::draw();
}
