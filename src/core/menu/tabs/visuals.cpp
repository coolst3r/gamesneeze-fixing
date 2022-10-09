#include "../menu.hpp"

const char *chamsMaterials[] = {"None", "Shaded", "Flat", "Screen Pulse", "Energy Ball", "Glow", "Plastic", "Darude", "Oil"};

void drawChamsWidget(
    const char *label,
    int *material,
    ImColor *color,
    int *overlayMaterial = nullptr,
    ImColor *overlayColor = nullptr,
    int *occludedMaterial = nullptr,
    ImColor *occludedColor = nullptr,
    int *ragdollMaterial = nullptr,
    ImColor *ragdollColor = nullptr,
    int *targetMaterial = nullptr,
    ImColor *targetColor = nullptr,
    int *backtrackMaterial = nullptr,
    ImColor *backtrackColor = nullptr,
    bool *backtrackTrail = nullptr,
    bool *wireframe = nullptr,
    bool *overlayWireframe = nullptr
) {
    char btnLabel[64];

    snprintf(btnLabel, sizeof(btnLabel), "Chams##%s", label);

    if (ImGui::Button(btnLabel)) {
        ImGui::OpenPopup(btnLabel);
    }

    if (ImGui::BeginPopup(btnLabel)) {
        ImGui::Text("%s Chams", label);
        ImGui::Separator();
        ImGui::Text("%s Chams", label);
        ImGui::Combo("##Chams Material", material, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
        ImGui::SameLine();
        ImGui::ColorEdit4("##Chams Color", (float *)color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);

        if (wireframe != nullptr) {
            ImGui::SameLine();
            ImGui::Checkbox("Wireframe ##Visible Wireframe", wireframe);
        }

        if (overlayMaterial != nullptr) {
            ImGui::Text("%s Chams Overlay", label);
            ImGui::Combo("##Chams Overlay Material", overlayMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            ImGui::SameLine();
            ImGui::ColorEdit4("Chams Overlay Color", (float *)overlayColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
        }

        if (overlayWireframe != nullptr) {
            ImGui::SameLine();
            ImGui::Checkbox("Wireframe ##Visible Overlay Wireframe", overlayWireframe);
        }

        if (occludedMaterial != nullptr) {
            ImGui::Separator();
            ImGui::Text("%s Occluded Chams", label);
            ImGui::Combo("##Occluded Chams Material", occludedMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            ImGui::SameLine();
            ImGui::ColorEdit4("Occluded Chams Color", (float *)occludedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
        }

        if (ragdollMaterial != nullptr) {
            ImGui::Separator();
            ImGui::Text("%s Ragdoll Chams", label);
            ImGui::Combo("##Ragdoll Chams Material", ragdollMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            ImGui::SameLine();
            ImGui::ColorEdit4("Ragdoll Chams Color", (float *)ragdollColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
        }

        if (targetMaterial != nullptr) {
            ImGui::Separator();
            ImGui::Text("%s Target Chams", label);
            ImGui::Combo("##Target Chams Material", targetMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            ImGui::SameLine();
            ImGui::ColorEdit4("Target Chams Color", (float *)targetColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
        }

        if (backtrackMaterial != nullptr) {
            ImGui::Separator();
            ImGui::Text("%s Backtrack Chams", label);
            ImGui::Combo("##Backtrack Chams Material", backtrackMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            ImGui::SameLine();
            ImGui::ColorEdit4("Backtrack Chams Color", (float *)backtrackColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::Checkbox("Trail", backtrackTrail);
        }

        ImGui::EndPopup();
    }
}

void impactsSelectBox(const char* configVarName) {
    ImGui::Text("Show Impacts");
    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());

    const char *selectedDisplay = "Off";
    int curSelected = CONFIGINT(configVarName);

    switch (curSelected) {
    // Client-only.
    case 3: {
        selectedDisplay = "Client-only";
        break;
    }
    // Server-only.
    case 2: {
        selectedDisplay = "Server-only";
        break;
    }
    // Both.
    case 1: {
        selectedDisplay = "Both";
        break;
    } }

    if (ImGui::BeginCombo("##HitBoxes", selectedDisplay)) {

        if (ImGui::Selectable("Off", curSelected == 0, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = 0;

        if (ImGui::Selectable("Client-only", curSelected == 3, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = 3;

        if (ImGui::Selectable("Server-only", curSelected == 2, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = 2;

        if (ImGui::Selectable("Both", curSelected == 1, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = 1;

        ImGui::EndCombo();
    }
}

void Menu::drawVisualsTab() {
    if (ImGui::BeginTabBar("##visTabs")) {
        if (ImGui::BeginTabItem("Players")) {
            ImGui::BeginChild("Teammates", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.325f, ImGui::GetWindowHeight() * 0.875f), true); {
                ImGui::Text("Teammates");
                ImGui::Separator();

                if (CONFIGBOOL("Visuals>Players>Teammates>Box")) {
                    ImGui::ColorEdit4("Box Color", (float*)&CONFIGCOL("Visuals>Players>Teammates>Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Box", &CONFIGBOOL("Visuals>Players>Teammates>Box"));

                if (CONFIGBOOL("Visuals>Players>Teammates>Skeleton")) {
                    ImGui::ColorEdit4("Skeleton Color", (float*)&CONFIGCOL("Visuals>Players>Teammates>Skeleton Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Skeleton", &CONFIGBOOL("Visuals>Players>Teammates>Skeleton"));

                ImGui::Checkbox("Name", &CONFIGBOOL("Visuals>Players>Teammates>Name"));
                ImGui::Checkbox("Health", &CONFIGBOOL("Visuals>Players>Teammates>Health"));

                if (CONFIGBOOL("Visuals>Players>Teammates>Health Bar")) {
                    ImGui::ColorEdit4("Health Bar Color", (float*)&CONFIGCOL("Visuals>Players>Teammates>Health Bar Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Health Bar", &CONFIGBOOL("Visuals>Players>Teammates>Health Bar"));

                if (CONFIGBOOL("Visuals>Players>Teammates>Health Bar")) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Dynamic Color", &CONFIGBOOL("Visuals>Players>Teammates>Dynamic Color"));
                }

                ImGui::Checkbox("Money", &CONFIGBOOL("Visuals>Players>Teammates>Money"));
                ImGui::Checkbox("Armor", &CONFIGBOOL("Visuals>Players>Teammates>Armor"));
                ImGui::Checkbox("Weapon", &CONFIGBOOL("Visuals>Players>Teammates>Weapon"));
                ImGui::Checkbox("Only When Dead", &CONFIGBOOL("Visuals>Players>Teammates>Only When Dead"));

                drawChamsWidget(
                    /* label */ "Teammates",
                    /* material */ &CONFIGINT("Visuals>Players>Teammates>Chams>Visible Material"),
                    /* color */ &CONFIGCOL("Visuals>Players>Teammates>Chams>Visible Color"),
                    /* overlayMaterial */ &CONFIGINT("Visuals>Players>Teammates>Chams>Visible Overlay Material"),
                    /* overlayColor */ &CONFIGCOL("Visuals>Players>Teammates>Chams>Visible Overlay Color"),
                    /* occludedMaterial */ &CONFIGINT("Visuals>Players>Teammates>Chams>Occluded Material"),
                    /* occludedColor */ &CONFIGCOL("Visuals>Players>Teammates>Chams>Occluded Color"),
                    /* ragdollMaterial */ &CONFIGINT("Visuals>Players>Teammates>Chams>Ragdoll Material"),
                    /* ragdollColor */ &CONFIGCOL("Visuals>Players>Teammates>Chams>Ragdoll Color"),
                    /* targetMaterial */ nullptr,
                    /* targetColor */ nullptr,
                    /* backtrackMaterial */ nullptr,
                    /* backtrackColor */ nullptr,
                    /* backtrackTrail */ nullptr,
                    /* wireframe */ &CONFIGBOOL("Visuals>Players>Teammates>Chams>Visible Wireframe"),
                    /* overlayWireframe */ &CONFIGBOOL("Visuals>Players>Teammates>Chams>Visible Overlay Wireframe")
                );

                ImGui::EndChild();
            }

            ImGui::SameLine();
            ImGui::BeginChild("Enemies", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.325f, ImGui::GetWindowHeight() * 0.875f), true); {
                ImGui::Text("Enemies");
                ImGui::Separator();

                if (CONFIGBOOL("Visuals>Players>Enemies>Box")) {
                    ImGui::ColorEdit4("Box Color", (float*)&CONFIGCOL("Visuals>Players>Enemies>Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Box", &CONFIGBOOL("Visuals>Players>Enemies>Box"));

                if (CONFIGBOOL("Visuals>Players>Enemies>Skeleton")) {
                    ImGui::ColorEdit4("Skeleton Color", (float*)&CONFIGCOL("Visuals>Players>Enemies>Skeleton Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Skeleton", &CONFIGBOOL("Visuals>Players>Enemies>Skeleton"));

                ImGui::Checkbox("Name", &CONFIGBOOL("Visuals>Players>Enemies>Name"));
                ImGui::Checkbox("Health", &CONFIGBOOL("Visuals>Players>Enemies>Health"));

                if(CONFIGBOOL("Visuals>Players>Enemies>Health Bar")) {
                    ImGui::ColorEdit4("Health Bar Color", (float*)&CONFIGCOL("Visuals>Players>Enemies>Health Bar Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Health Bar", &CONFIGBOOL("Visuals>Players>Enemies>Health Bar"));

                if(CONFIGBOOL("Visuals>Players>Enemies>Health Bar")) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Dynamic Color", &CONFIGBOOL("Visuals>Players>Enemies>Dynamic Color"));
                }

                ImGui::Checkbox("Money", &CONFIGBOOL("Visuals>Players>Enemies>Money"));
                ImGui::Checkbox("Armor", &CONFIGBOOL("Visuals>Players>Enemies>Armor"));
                ImGui::Checkbox("Flashed", &CONFIGBOOL("Visuals>Players>Enemies>Flashed"));
                ImGui::Checkbox("Weapon", &CONFIGBOOL("Visuals>Players>Enemies>Weapon"));
                ImGui::Checkbox("Radar", &CONFIGBOOL("Visuals>Players>Enemies>Radar"));
                ImGui::Checkbox("Forwardtrack Dots", &CONFIGBOOL("Visuals>Players>Enemies>Forwardtrack Dots"));
                ImGui::Checkbox("Vis Check", &CONFIGBOOL("Visuals>Players>Enemies>Vis Check"));
                ImGui::Checkbox("Only When Dead", &CONFIGBOOL("Visuals>Players>Enemies>Only When Dead"));

                drawChamsWidget(
                    /* label */ "Enemies",
                    /* material */ &CONFIGINT("Visuals>Players>Enemies>Chams>Visible Material"),
                    /* color */ &CONFIGCOL("Visuals>Players>Enemies>Chams>Visible Color"),
                    /* overlayMaterial */ &CONFIGINT("Visuals>Players>Enemies>Chams>Visible Overlay Material"),
                    /* overlayColor */ &CONFIGCOL("Visuals>Players>Enemies>Chams>Visible Overlay Color"),
                    /* occludedMaterial */ &CONFIGINT("Visuals>Players>Enemies>Chams>Occluded Material"),
                    /* occludedColor */ &CONFIGCOL("Visuals>Players>Enemies>Chams>Occluded Color"),
                    /* ragdollMaterial */ &CONFIGINT("Visuals>Players>Enemies>Chams>Ragdoll Material"),
                    /* ragdollColor */ &CONFIGCOL("Visuals>Players>Enemies>Chams>Ragdoll Color"),
                    /* targetMaterial */ &CONFIGINT("Visuals>Players>Enemies>Chams>Target Material"),
                    /* targetColor */ &CONFIGCOL("Visuals>Players>Enemies>Chams>Target Color"),
                    /* backtrackMaterial */ &CONFIGINT("Visuals>Players>Enemies>Chams>Backtrack Material"),
                    /* backtrackColor */ &CONFIGCOL("Visuals>Players>Enemies>Chams>Backtrack Color"),
                    /* backtrackTrail */ &CONFIGBOOL("Visuals>Players>Enemies>Chams>Trail"),
                    /* wireframe */ &CONFIGBOOL("Visuals>Players>Enemies>Chams>Visible Wireframe"),
                    /* overlayWireframe */ &CONFIGBOOL("Visuals>Players>Enemies>Chams>Visible Overlay Wireframe")
                );

                ImGui::EndChild();
            }
            ImGui::SameLine();
            ImGui::BeginChild("LocalPlayer", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.325f, ImGui::GetWindowHeight() * 0.875f), true); {
                ImGui::Text("LocalPlayer");
                ImGui::Separator();
                ImGui::Text("Arms");

                drawChamsWidget(
                    /* label */ "Arms",
                    /* material */ &CONFIGINT("Visuals>Players>LocalPlayer>Arms Material"),
                    /* color */ &CONFIGCOL("Visuals>Players>LocalPlayer>Arms Color"),
                    /* overlayMaterial */ &CONFIGINT("Visuals>Players>LocalPlayer>Arms Overlay Material"),
                    /* overlayColor */ &CONFIGCOL("Visuals>Players>LocalPlayer>Arms Overlay Color"),
                    /* occludedMaterial */ nullptr,
                    /* occludedColor */ nullptr,
                    /* ragdollMaterial */ nullptr,
                    /* ragdollColor */ nullptr,
                    /* targetMaterial */ nullptr,
                    /* targetColor */ nullptr,
                    /* backtrackMaterial */ nullptr,
                    /* backtrackColor */ nullptr,
                    /* backtrackTrail */ nullptr,
                    /* wireframe */ &CONFIGBOOL("Visuals>Players>LocalPlayer>Arms Wireframe"),
                    /* overlayWireframe */ &CONFIGBOOL("Visuals>Players>LocalPlayer>Arms Overlay Wireframe")
                );

                ImGui::Separator();
                ImGui::Text("Sleeves");

                drawChamsWidget(
                    /* label */ "Sleeves",
                    /* material */ &CONFIGINT("Visuals>Players>LocalPlayer>Sleeve Material"),
                    /* color */ &CONFIGCOL("Visuals>Players>LocalPlayer>Sleeve Color"),
                    /* overlayMaterial */ &CONFIGINT("Visuals>Players>LocalPlayer>Sleeve Overlay Material"),
                    /* overlayColor */ &CONFIGCOL("Visuals>Players>LocalPlayer>Sleeve Overlay Color"),
                    /* occludedMaterial */ nullptr,
                    /* occludedColor */ nullptr,
                    /* ragdollMaterial */ nullptr,
                    /* ragdollColor */ nullptr,
                    /* targetMaterial */ nullptr,
                    /* targetColor */ nullptr,
                    /* backtrackMaterial */ nullptr,
                    /* backtrackColor */ nullptr,
                    /* backtrackTrail */ nullptr,
                    /* wireframe */ &CONFIGBOOL("Visuals>Players>LocalPlayer>Sleeve Wireframe"),
                    /* overlayWireframe */ &CONFIGBOOL("Visuals>Players>LocalPlayer>Sleeve Overlay Wireframe")
                );

                ImGui::Separator();
                ImGui::Text("Weapons");

                drawChamsWidget(
                    /* label */ "Weapons",
                    /* material */ &CONFIGINT("Visuals>Players>LocalPlayer>Weapon Material"),
                    /* color */ &CONFIGCOL("Visuals>Players>LocalPlayer>Weapon Color"),
                    /* overlayMaterial */ &CONFIGINT("Visuals>Players>LocalPlayer>Weapon Overlay Material"),
                    /* overlayColor */ &CONFIGCOL("Visuals>Players>LocalPlayer>Weapon Overlay Color"),
                    /* occludedMaterial */ nullptr,
                    /* occludedColor */ nullptr,
                    /* ragdollMaterial */ nullptr,
                    /* ragdollColor */ nullptr,
                    /* targetMaterial */ nullptr,
                    /* targetColor */ nullptr,
                    /* backtrackMaterial */ nullptr,
                    /* backtrackColor */ nullptr,
                    /* backtrackTrail */ nullptr,
                    /* wireframe */ &CONFIGBOOL("Visuals>Players>LocalPlayer>Weapon Wireframe"),
                    /* overlayWireframe */ &CONFIGBOOL("Visuals>Players>LocalPlayer>Weapon Overlay Wireframe")
                );

                ImGui::Separator();
                ImGui::ColorEdit4("Crosshair Color", (float*)&CONFIGCOL("Visuals>Players>LocalPlayer>Crosshair Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SameLine();
                ImGui::ColorEdit4("Crosshair Border Color", (float*)&CONFIGCOL("Visuals>Players>LocalPlayer>Crosshair Border Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SameLine();
                ImGui::Checkbox("Spread Crosshair", &CONFIGBOOL("Visuals>Players>LocalPlayer>Spread Crosshair"));
                ImGui::Checkbox("Recoil Crosshair", &CONFIGBOOL("Visuals>Players>LocalPlayer>Recoil Crosshair"));
                // Make sure they can't both be on at the same time
                if (CONFIGBOOL("Visuals>Players>LocalPlayer>Recoil Crosshair") && !CONFIGBOOL("Visuals>Players>LocalPlayer>Spread Crosshair")) {
                    ImGui::Checkbox("Only When Shooting", &CONFIGBOOL("Visuals>Players>LocalPlayer>Recoil Crosshair>Only When Shooting"));
                }
                ImGui::Separator();
                ImGui::Checkbox("No Aim Punch", &CONFIGBOOL("Visuals>Players>LocalPlayer>No Aim Punch"));
                ImGui::Checkbox("No View Punch", &CONFIGBOOL("Visuals>Players>LocalPlayer>No View Punch"));
                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("World")) {
            ImGui::BeginChild("World", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.65f, ImGui::GetWindowHeight() * 0.875f), true); {
                ImGui::Text("World");
                ImGui::Separator();

                if (
                  ImGui::ColorEdit4("World Color Modulation", (float*)&CONFIGCOL("Visuals>World>World>World Color Modulation"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel) ||
                  ImGui::ColorEdit4("SkyBox Color Modulation", (float*)&CONFIGCOL("Visuals>World>World>SkyBox Color Modulation"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel) ||
                  ImGui::Button("Update Color Modulation")) {
                    Features::ColorModulation::updateColorModulation();
                }

                ImGui::Text("NightMode");
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::SliderInt("##NightMode", &CONFIGINT("Visuals>World>World>Nightmode"), 0, 100);
                ImGui::Text("Bloom");
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::SliderInt("##Bloom", &CONFIGINT("Visuals>World>World>Bloom"), 0, 1000);
                ImGui::Text("Skybox");
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::Combo("##Skybox", &CONFIGINT("Visuals>World>World>Skybox"), skyboxes, IM_ARRAYSIZE(skyboxes));
                ImGui::Text("Camera FOV");
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::SliderInt("##Camera FOV", &CONFIGINT("Visuals>World>World>FOV"), 70, 120);
                ImGui::Text("Viewmodel FOV");
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::SliderInt("##Viewmodel FOV", &CONFIGINT("Visuals>World>World>Viewmodel FOV"), 0, 130);
                ImGui::Checkbox("Third Person", &CONFIGBOOL("Visuals>World>World>Third Person"));
                ImGui::Checkbox("No Sniper Crosshair", &CONFIGBOOL("Visuals>World>World>No Sniper Crosshair"));
                ImGui::Checkbox("No Flash", &CONFIGBOOL("Visuals>World>World>No Flash"));
                ImGui::Text("Flash Amount");
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::SliderInt("##Flash Amount", &CONFIGINT("Visuals>World>World>No Flash Amount"), 0, 255);
                ImGui::Checkbox("Fullbright (Applying this setting may lag momentarily)", &CONFIGBOOL("Visuals>World>World>Fullbright"));
                ImGui::Checkbox("Draw Gray (Applying this setting may lag momentarily)", &CONFIGBOOL("Visuals>World>World>Gray"));
                ImGui::Checkbox("Low-res Image (Applying this setting may lag momentarily)", &CONFIGBOOL("Visuals>World>World>Low-res Image"));
                ImGui::Checkbox("Ragdoll Gravity", &CONFIGBOOL("Visuals>World>World>Ragdoll Gravity"));
                ImGui::Checkbox("No Shadows (CSM) (Applying this setting may lag momentarily)", &CONFIGBOOL("Visuals>World>World>No Shadows"));

                impactsSelectBox("Visuals>World>World>Show Impacts");

                if (CONFIGBOOL("Visuals>World>World>Bullet Tracers")) {
                    ImGui::ColorEdit4("Bullet Tracers Color", (float*)&CONFIGCOL("Visuals>World>World>Bullet Tracers Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Bullet Tracers", &CONFIGBOOL("Visuals>World>World>Bullet Tracers"));

                if (CONFIGBOOL("Visuals>World>World>Bullet Tracers")) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Laser", &CONFIGBOOL("Visuals>World>World>Bullet Tracers Laser"));
                }
         
                ImGui::Separator();
                ImGui::Checkbox("Fog", &CONFIGBOOL("Visuals>World>World>Override Fog"));

                if (CONFIGBOOL("Visuals>World>World>Override Fog")) {
                    auto fogStartValue = CONFIGINT("Visuals>World>World>Fog Start");
                    auto fogEndValue = CONFIGINT("Visuals>World>World>Fog End");
                    auto fogFarzValue = CONFIGINT("Visuals>World>World>Fog Farz");

                    ImGui::Text("Fog Start");
                    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                    ImGui::SliderInt("##Fog Start", &fogStartValue, 0, 30000);
                    ImGui::Text("Fog End");
                    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                    ImGui::SliderInt("##Fog End", &fogEndValue, 0, 30000);
                    ImGui::Text("Fog Farz (Clip)");
                    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                    ImGui::SliderInt("##Fog Farz", &fogFarzValue, 0, 30000);
                    ImGui::ColorEdit4("Fog Color", (float*)&CONFIGCOL("Visuals>World>World>Fog Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
                
                    CONFIGINT("Visuals>World>World>Fog Start") = std::clamp(fogStartValue, 0, fogEndValue);
                    CONFIGINT("Visuals>World>World>Fog End") = std::clamp(fogEndValue, fogStartValue, fogFarzValue);
                    CONFIGINT("Visuals>World>World>Fog Farz") = std::clamp(fogFarzValue, fogEndValue, 30000);
                }
                
                ImGui::EndChild();
            }
            ImGui::SameLine();
            ImGui::BeginChild("Items", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.34f, ImGui::GetWindowHeight() * 0.875f), true); {
                ImGui::Text("Items");
                ImGui::Separator();
                if (CONFIGBOOL("Visuals>World>Items>Weapon Box")) {
                    ImGui::ColorEdit4("Weapon Box Color", (float*)&CONFIGCOL("Visuals>World>Items>Weapon Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }
                ImGui::Checkbox("Weapon Box", &CONFIGBOOL("Visuals>World>Items>Weapon Box"));
                ImGui::Checkbox("Weapon Label", &CONFIGBOOL("Visuals>World>Items>Weapon Label"));
                ImGui::Separator();
                if (CONFIGBOOL("Visuals>World>Items>Grenade Box")) {
                    ImGui::ColorEdit4("Grenade Box Color", (float*)&CONFIGCOL("Visuals>World>Items>Grenade Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }
                ImGui::Checkbox("Grenade Box", &CONFIGBOOL("Visuals>World>Items>Grenade Box"));
                if (CONFIGBOOL("Visuals>World>Items>Grenade Box")) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Dynamic Color", &CONFIGBOOL("Visuals>World>Items>Grenade Box Dynamic Color"));
                }
                ImGui::Checkbox("Grenade Label", &CONFIGBOOL("Visuals>World>Items>Grenade Label"));
                ImGui::Checkbox("Grenade Owners", &CONFIGBOOL("Visuals>World>Items>Grenade Owners"));
                ImGui::Separator();
                if (CONFIGBOOL("Visuals>World>Items>Planted C4 Box")) {
                    ImGui::ColorEdit4("Planted C4 Box Color", (float*)&CONFIGCOL("Visuals>World>Items>Planted C4 Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }
                ImGui::Checkbox("Planted C4 Box", &CONFIGBOOL("Visuals>World>Items>Planted C4 Box"));
                ImGui::Checkbox("Planted C4 Label", &CONFIGBOOL("Visuals>World>Items>Planted C4 Label"));
                ImGui::Separator();
                if (CONFIGBOOL("Visuals>World>Items>Chicken Box")) {
                    ImGui::ColorEdit4("Chicken Box Color", (float*)&CONFIGCOL("Visuals>World>Items>Chicken Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }
                ImGui::Checkbox("Chicken Box", &CONFIGBOOL("Visuals>World>Items>Chicken Box"));
                ImGui::Checkbox("Chicken Label", &CONFIGBOOL("Visuals>World>Items>Chicken Label"));
                ImGui::Separator();
                if (CONFIGBOOL("Visuals>World>Items>Fish Box")) {
                    ImGui::ColorEdit4("Fish Box Color", (float*)&CONFIGCOL("Visuals>World>Items>Fish Box Color"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                    ImGui::SameLine();
                }
                ImGui::Checkbox("Fish Box", &CONFIGBOOL("Visuals>World>Items>Fish Box"));
                ImGui::Checkbox("Fish Label", &CONFIGBOOL("Visuals>World>Items>Fish Label"));
                ImGui::Separator();

                ImGui::Checkbox("ESP Quite literally everything", &CONFIGBOOL("Visuals>World>Items>ESP Quite literally everything"));

                ImGui::Separator();
                ImGui::Text("World Models");

                drawChamsWidget(
                    /* label */ "World Model",
                    /* material */ &CONFIGINT("Visuals>World>World>Model Material"),
                    /* color */ &CONFIGCOL("Visuals>World>World>Model Color"),
                    /* overlayMaterial */ nullptr,
                    /* overlayColor */ nullptr,
                    /* occludedMaterial */ nullptr,
                    /* occludedColor */ nullptr,
                    /* ragdollMaterial */ nullptr,
                    /* ragdollColor */ nullptr,
                    /* targetMaterial */ nullptr,
                    /* targetColor */ nullptr,
                    /* backtrackMaterial */ nullptr,
                    /* backtrackColor */ nullptr,
                    /* backtrackTrail */ nullptr,
                    /* wireframe */ nullptr,
                    /* overlayWireframe */ nullptr
                );

                drawChamsWidget(
                    /* label */ "World Overlay Model",
                    /* material */ &CONFIGINT("Visuals>World>World>Model Overlay Material"),
                    /* color */ &CONFIGCOL("Visuals>World>World>Model Overlay Color"),
                    /* overlayMaterial */ nullptr,
                    /* overlayColor */ nullptr,
                    /* occludedMaterial */ nullptr,
                    /* occludedColor */ nullptr,
                    /* ragdollMaterial */ nullptr,
                    /* ragdollColor */ nullptr,
                    /* targetMaterial */ nullptr,
                    /* targetColor */ nullptr,
                    /* backtrackMaterial */ nullptr,
                    /* backtrackColor */ nullptr,
                    /* backtrackTrail */ nullptr,
                    /* wireframe */ nullptr,
                    /* overlayWireframe */ nullptr
                );

                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Menu")) {
            ImGui::BeginChild("Menu", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.65f, ImGui::GetWindowHeight() * 0.875f), true); {
                ImGui::Text("Menu");
                ImGui::Separator();

                ImGui::ColorEdit4("Accent", (float *)&CONFIGCOL("Visuals>Menu>Menu>Accent"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SameLine();
                ImGui::Text("Accent");
                ImGui::ColorEdit4("Background", (float *)&CONFIGCOL("Visuals>Menu>Menu>Background"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SameLine();
                ImGui::Text("Background");
                ImGui::ColorEdit4("Background Secondary", (float *)&CONFIGCOL("Visuals>Menu>Menu>Background Secondary"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SameLine();
                ImGui::Text("Background Secondary");
                ImGui::ColorEdit4("Foreground", (float *)&CONFIGCOL("Visuals>Menu>Menu>Foreground"), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::SameLine();
                ImGui::Text("Foreground");
                
                ImGui::EndChild();
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}
