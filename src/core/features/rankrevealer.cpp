#include "features.hpp"
#include "../../sdk/sdk.hpp"
 
void Features::RankReveal::createMove(Command* cmd) {
    if (cmd->tickCount % 64 == 0 && !(cmd->buttons & (1 << 16)/*IN_SCORE*/) && CONFIGBOOL("Misc>Misc>Misc>Rank Revealer")) {
        Interfaces::client->DispatchUserMessage(50, 0, 0, nullptr);
    }
}
