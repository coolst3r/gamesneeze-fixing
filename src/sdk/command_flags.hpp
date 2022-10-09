#pragma once

namespace sdk {
    namespace command_flags {
        constexpr int ATTACK = 1 << 0;
        constexpr int JUMP = 1 << 1;
        constexpr int DUCK = 1 << 2;
        constexpr int FORWARD = 1 << 3;
        constexpr int BACK = 1 << 4;
        constexpr int USE = 1 << 5;
        constexpr int MOVELEFT = 1 << 9;
        constexpr int MOVERIGHT = 1 << 10;
        constexpr int ATTACK2 = 1 << 11;
        constexpr int SCORE = 1 << 16;
        constexpr int BULLRUSH = 1 << 22;
    }
}

