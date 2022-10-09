#include "../../includes.hpp"
#include "hooks.hpp"

void Hooks::DrawModelExecute::hook(
    void *thisptr,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *pCustomBoneToWorld
) {
    Features::Chams::drawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
}
