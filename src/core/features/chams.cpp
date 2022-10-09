#include "../../includes.hpp"
#include "../../sdk/classes/keyvalues.hpp"
#include "features.hpp"
#include <array>
#include <cstring>

inline bool materialsInit;
inline std::array<IMaterial *, 9> materials;

inline IMaterial *createMaterial(const char *materialName, const char *materialType, const char *material) {
	auto keyValues = new KeyValues(materialName);

	Offsets::initKeyValues(keyValues, materialType, 0, 0);
	Offsets::loadFromBuffer(keyValues, materialName, material, nullptr, nullptr, nullptr);

	return Interfaces::materialSystem->createMaterial(materialName, keyValues);
}

inline void createMaterials() {
    if (!materialsInit) {
        materials[0] = Interfaces::materialSystem->findMaterial("debug/debugambientcube");
        materials[1] = Interfaces::materialSystem->findMaterial("debug/debugdrawflat");
        materials[2] = Interfaces::materialSystem->findMaterial("dev/screenhighlight_pulse");
        materials[3] = Interfaces::materialSystem->findMaterial("effects/energyball");

        materials[4] = createMaterial("glow", "VertexLitGeneric",
        R"#("VertexLitGeneric" {
            "$additive" "1"
            "$envmap" "models/effects/cube_white"
            "$envmaptint" "[1 1 1]"
            "$envmapfresnel" "1"
            "$envmapfresnelminmaxexp" "[0 1 2]"
            "$alpha" "0.8"
        })#");

        materials[5] = Interfaces::materialSystem->findMaterial("models/inventory_items/trophy_majors/gloss");
        materials[6] = Interfaces::materialSystem->findMaterial("models/inventory_items/music_kit/darude_01/mp3_detail");

        materials[7] = createMaterial("pearlescent", "VertexLitGeneric",
        R"#("VertexLitGeneric"
        {
            "$basetexture" "vgui/white_additive"
            "$nocull" "1"
            "$nofog" "1"
            "$model" "1"
            "$nocull" "0"
            "$phong" "1"
            "$phongboost" "0"
            "$basemapalphaphongmask" "1"
            "$pearlescent" "6"
        })#");

        materialsInit = true;
    }
}

inline void cham(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones,
    ImColor color,
    int materialIndex,
    bool wireframe,
    bool ignoreZ = false,
    bool overlay = false
) {
    if ((materialIndex < 1 || materialIndex > 8)) {
        if (!overlay) {
            Hooks::DrawModelExecute::original(self, ctx, state, pInfo, bones);
        }

        return;
    }

    auto material = materials[materialIndex - 1];

    material->ignoreZ(ignoreZ);
    material->wireframe(wireframe);
    material->setColor(color);

    Interfaces::modelRender->ForcedMaterialOverride(material);
    Hooks::DrawModelExecute::original(self, ctx, state, pInfo, bones);
    Interfaces::modelRender->ForcedMaterialOverride(0);
}

inline void cham(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones,
    const char *colorName,
    const char *materialName,
    const char *wireframeName = nullptr,
    bool ignoreZ = false,
    bool overlay = false
) {
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* color */ colorName == nullptr ? ImColor{0.0f, 0.0f, 0.0f, 1.0f} : CONFIGCOL(colorName),
        /* materialIndex */ materialName == nullptr ? 0 : CONFIGINT(materialName),
        /* wireframe */ wireframeName == nullptr ? false : CONFIGBOOL(materialName),
        /* ignoreZ */ ignoreZ,
        /* overlay */ overlay
    );
}

inline void chamPlayerLayer(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones,
    const char *colorName,
    const char *materialName,
    const char *wireframeName,
    const char *occludedColorName,
    const char *occludedMaterialName,
    const char *overlayColorName,
    const char *overlayMaterialName,
    const char *overlayWireframeName
) {
    // ignorez player
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ occludedColorName,
        /* materialName */ occludedMaterialName,
        /* wireframeName */ nullptr,
        /* ignoreZ */ true,
        /* overlay */ true
    );

    // visible player
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ colorName,
        /* materialName */ materialName,
        /* wireframeName */ wireframeName
    );

    // visible player overlay
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ overlayColorName,
        /* materialName */ overlayMaterialName,
        /* wireframeName */ overlayWireframeName,
        /* ignoreZ */ false,
        /* overlay */ true
    );
}

inline void chamPlayer(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones,
    Player *player,
    const char *colorName,
    const char *materialName,
    const char *wireframeName,
    const char *occludedColorName,
    const char *occludedMaterialName,
    const char *overlayColorName,
    const char *overlayMaterialName,
    const char *overlayWireframeName,
    const char *targetColorName,
    const char *targetMaterialName,
    const char *ragdollColorName,
    const char *ragdollMaterialName
) {
    if (player->health() == 0) {
        cham(
            /* self */ self,
            /* ctx */ ctx,
            /* state */ state,
            /* pInfo */ pInfo,
            /* bones */ bones,
            /* colorName */ ragdollColorName,
            /* materialName */ ragdollMaterialName
        );

        return;
    }

    auto isTarget = false; //player == Features::RageBot::target;

    if (targetColorName != nullptr && targetMaterialName != nullptr && isTarget) {
        chamPlayerLayer(
            /* self */ self,
            /* ctx */ ctx,
            /* state */ state,
            /* pinfo */ pInfo,
            /* bones */ bones,
    	    /* colorName */ targetColorName,
    	    /* materialName */ targetMaterialName,
    	    /* wireframeName */ nullptr,
            /* occludedColorName */ targetColorName,
            /* occludedMaterialName */ targetMaterialName,
            /* overlayColorName */ targetColorName,
            /* overlayMaterialName */ targetMaterialName,
    	    /* overlayWireframeName */ nullptr
        );
    } else {
        chamPlayerLayer(
            /* self */ self,
            /* ctx */ ctx,
            /* state */ state,
            /* pinfo */ pInfo,
            /* bones */ bones,
    	    /* colorName */ colorName,
    	    /* materialName */ materialName,
    	    /* wireframeName */ wireframeName,
            /* occludedColorName */ occludedColorName,
            /* occludedMaterialName */ occludedMaterialName,
            /* overlayColorName */ overlayColorName,
            /* overlayMaterialName */ overlayMaterialName,
    	    /* overlayWireframeName */ overlayWireframeName
        );
    }
}

void chamEnemy(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones,
    Player *player
) {
    chamPlayer(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* player */ player,
        /* colorName */ "Visuals>Players>Enemies>Chams>Visible Color",
        /* materialName */ "Visuals>Players>Enemies>Chams>Visible Material",
        /* wireframeName */ "Visuals>Players>Enemies>Chams>Visible Wireframe",
        /* occludedColorName */ "Visuals>Players>Enemies>Chams>Occluded Color",
        /* occludedMaterialName */ "Visuals>Players>Enemies>Chams>Occluded Material",
        /* overlayColorName */ "Visuals>Players>Enemies>Chams>Visible Overlay Color",
        /* overlayMaterialName */ "Visuals>Players>Enemies>Chams>Visible Overlay Material",
        /* overlayWireframeName */ "Visuals>Players>Enemies>Chams>Visible Overlay Wireframe",
        /* targetColorName */ "Visuals>Players>Enemies>Chams>Target Color",
        /* targetMaterialName */ "Visuals>Players>Enemies>Chams>Target Material",
        /* ragdollColorName */ "Visuals>Players>Enemies>Chams>Ragdoll Color",
        /* ragdollMaterialName */ "Visuals>Players>Enemies>Chams>Ragdoll Material"
    );
}

void chamTeammate(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones,
    Player *player
) {
    chamPlayer(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* player */ player,
        /* colorName */ "Visuals>Players>Teammates>Chams>Visible Color",
        /* materialName */ "Visuals>Players>Teammates>Chams>Visible Material",
        /* wireframeName */ "Visuals>Players>Teammates>Chams>Visible Wireframe",
        /* occludedColorName */ "Visuals>Players>Teammates>Chams>Occluded Color",
        /* occludedMaterialName */ "Visuals>Players>Teammates>Chams>Occluded Material",
        /* overlayColorName */ "Visuals>Players>Teammates>Chams>Visible Overlay Color",
        /* overlayMaterialName */ "Visuals>Players>Teammates>Chams>Visible Overlay Material",
        /* overlayWireframeName */ "Visuals>Players>Teammates>Chams>Visible Overlay Wireframe",
        /* targetColorName */ nullptr,
        /* targetMaterialName */ nullptr,
        /* ragdollColorName */ "Visuals>Players>Teammates>Chams>Ragdoll Color",
        /* ragdollMaterialName */ "Visuals>Players>Teammates>Chams>Ragdoll Material"
    );
}

void chamPlayer(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones
) {
    auto player = Interfaces::entityList->player(pInfo.entity_index);

    if (player == nullptr) {
        return;
    }

    if (player->enemy()) {
        chamEnemy(self, ctx, state, pInfo, bones, player);
    } else {
        chamTeammate(self, ctx, state, pInfo, bones, player);
    }
}

void chamArms(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones
) {
    // arms
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ "Visuals>Players>LocalPlayer>Arms Color",
        /* materialName */ "Visuals>Players>LocalPlayer>Arms Material",
        /* wireframeName */ "Visuals>Players>LocalPlayer>Arms Wireframe"
    );

    // arms overlay
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ "Visuals>Players>LocalPlayer>Arms Overlay Color",
        /* materialName */ "Visuals>Players>LocalPlayer>Arms Overlay Material",
        /* wireframeName */ "Visuals>Players>LocalPlayer>Arms Overlay Wireframe",
        /* ignoreZ */ false,
        /* overlay */ true
    );
}

void chamSleeves(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones
) {
    // sleeves
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ "Visuals>Players>LocalPlayer>Sleeve Color",
        /* materialName */ "Visuals>Players>LocalPlayer>Sleeve Material",
        /* wireframeName */ "Visuals>Players>LocalPlayer>Sleeve Wireframe"
    );

    // sleeves overlay
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ "Visuals>Players>LocalPlayer>Sleeve Overlay Color",
        /* materialName */ "Visuals>Players>LocalPlayer>Sleeve Overlay Material",
        /* wireframeName */ "Visuals>Players>LocalPlayer>Sleeve Overlay Wireframe",
        /* ignoreZ */ false,
        /* overlay */ true
    );
}

void chamWeapon(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones
) {
    if (Globals::localPlayer == nullptr) {
        Hooks::DrawModelExecute::original(self, ctx, state, pInfo, bones);

        return;
    }

    if (Globals::localPlayer->scoped()) {
        Hooks::DrawModelExecute::original(self, ctx, state, pInfo, bones);

        return;
    }

    // weapon
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ "Visuals>Players>LocalPlayer>Weapon Color",
        /* materialName */ "Visuals>Players>LocalPlayer>Weapon Material",
        /* wireframeName */ "Visuals>Players>LocalPlayer>Weapon Wireframe"
    );

    // weapon overlay
    cham(
        /* self */ self,
        /* ctx */ ctx,
        /* state */ state,
        /* pInfo */ pInfo,
        /* bones */ bones,
        /* colorName */ "Visuals>Players>LocalPlayer>Weapon Overlay Color",
        /* materialName */ "Visuals>Players>LocalPlayer>Weapon Overlay Material",
        /* wireframeName */ "Visuals>Players>LocalPlayer>Weapon Overlay Wireframe",
        /* ignoreZ */ false,
        /* overlay */ true
    );
}

void Features::Chams::drawModelExecute(
    void *self,
    void *ctx,
    const DrawModelState_t &state,
    const ModelRenderInfo_t &pInfo,
    Matrix3x4 *bones
) {
    createMaterials();

	const char *modelName = Interfaces::modelInfo->GetModelName(pInfo.pModel);

	if (strstr(modelName, "models/player")) {
        if (!strstr(modelName, "shadow")) {
            chamPlayer(self, ctx, state, pInfo, bones);
        }
    } else if (strstr(modelName, "models/weapons/v_")) {
        if (strstr(modelName, "sleeve")) {
            chamSleeves(self, ctx, state, pInfo, bones);
        } else if (strstr(modelName, "arms")) {
            chamArms(self, ctx, state, pInfo, bones);
        } else {
            chamWeapon(self, ctx, state, pInfo, bones);
        }
    } else if (strstr(modelName, "models/weapons/v_")) {
        chamWeapon(self, ctx, state, pInfo, bones);
    } else {
        cham(
            /* self */ self,
            /* ctx */ ctx,
            /* state */ state,
            /* pInfo */ pInfo,
            /* bones */ bones,
            /* colorName */ "Visuals>World>World>Model Color",
            /* materialName */ "Visuals>World>World>Model Material"
        );

        cham(
            /* self */ self,
            /* ctx */ ctx,
            /* state */ state,
            /* pInfo */ pInfo,
            /* bones */ bones,
            /* colorName */ "Visuals>World>World>Model Overlay Color",
            /* materialName */ "Visuals>World>World>Model Overlay Material"
        );
    }
}
