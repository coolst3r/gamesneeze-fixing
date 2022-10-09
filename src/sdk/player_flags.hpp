#pragma once

namespace sdk {
    namespace player_flags {
        constexpr int ON_GROUND = 1 << 0; 
        constexpr int DUCKING = 1 << 1; 
        constexpr int FROZEN = 1 << 5; 
        constexpr int CLIENT = 1 << 7; 
        constexpr int FAKE_CLIENT = 1 << 8; 
        constexpr int ON_PARTIAL_GROUND = 1 << 18; 
    }
}
