#include "../menu.hpp"
#include <filesystem>
#include "../config.hpp"

void primaryWeaponSelectBox(const char* configVarName) {
    ImGui::Text("Primary Weapon");
    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());

    auto curSelected = (PrimaryWeapon)CONFIGINT(configVarName);
    const char *selected = "None";

    switch (curSelected) {
    case PrimaryWeapon::AUG: {
        selected = "AUG/SG 553";
        break;
    }
    case PrimaryWeapon::AWP: {
        selected = "AWP";
        break;
    }
    case PrimaryWeapon::FAMAS: {
        selected = "FAMASi/Galil AR";
        break;
    }
    case PrimaryWeapon::M249: {
        selected = "M249";
        break;
    }
    case PrimaryWeapon::M4A4: {
        selected = "M4A4/AK-47";
        break;
    }
    case PrimaryWeapon::MAG7: {
        selected = "MAG7/Sawed-Off";
        break;
    }
    case PrimaryWeapon::MP7: {
        selected = "MP7";
        break;
    }
    case PrimaryWeapon::MP9: {
        selected = "MP9/MAC-10";
        break;
    }
    case PrimaryWeapon::Negev: {
        selected = "Negev";
        break;
    }
    case PrimaryWeapon::Nova: {
        selected = "Nova";
        break;
    }
    case PrimaryWeapon::P90: {
        selected = "P90";
        break;
    }
    case PrimaryWeapon::PPBizon: {
        selected = "PP-Bizon";
        break;
    }
    case PrimaryWeapon::SCAR20: {
        selected = "SCAR-20/G3SG1";
        break;
    }
    case PrimaryWeapon::SSG08: {
        selected = "SSG-08";
        break;
    }
    case PrimaryWeapon::UMP45: {
        selected = "UMP-45";
        break;
    }
    case PrimaryWeapon::XM1014: {
        selected = "XM1014";
        break;
    }
    default: {
        break;
    } }

    if (ImGui::BeginCombo("##Primary Weapon", selected)) {

        if (ImGui::Selectable("None", curSelected == PrimaryWeapon::None, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::None;

        if (ImGui::Selectable("AUG/SG 553", curSelected == PrimaryWeapon::AUG, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::AUG;

        if (ImGui::Selectable("AWP", curSelected == PrimaryWeapon::AWP, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::AWP;

        if (ImGui::Selectable("FAMAS/Galil AR", curSelected == PrimaryWeapon::FAMAS, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::FAMAS;

        if (ImGui::Selectable("M249", curSelected == PrimaryWeapon::M249, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::M249;

        if (ImGui::Selectable("M4A4/AK-47", curSelected == PrimaryWeapon::M4A4, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::M4A4;

        if (ImGui::Selectable("MAG7/Sawed-Off", curSelected == PrimaryWeapon::MAG7, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::MAG7;

        if (ImGui::Selectable("MP7", curSelected == PrimaryWeapon::MP7, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::MP7;

        if (ImGui::Selectable("MP9/MAC-10", curSelected == PrimaryWeapon::MP9, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::MP9;

        if (ImGui::Selectable("Negev", curSelected == PrimaryWeapon::Negev, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::Negev;

        if (ImGui::Selectable("Nova", curSelected == PrimaryWeapon::Nova, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::Nova;

        if (ImGui::Selectable("P90", curSelected == PrimaryWeapon::P90, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::P90;

        if (ImGui::Selectable("PP-Bizon", curSelected == PrimaryWeapon::PPBizon, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::PPBizon;

        if (ImGui::Selectable("SCAR-20/G3SG1", curSelected == PrimaryWeapon::SCAR20, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::SCAR20;

        if (ImGui::Selectable("SSG-08", curSelected == PrimaryWeapon::SSG08, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::SSG08;

        if (ImGui::Selectable("UMP-45", curSelected == PrimaryWeapon::UMP45, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::UMP45;

        if (ImGui::Selectable("XM1014", curSelected == PrimaryWeapon::XM1014, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)PrimaryWeapon::XM1014;

        ImGui::EndCombo();
    }
}

void secondaryWeaponSelectBox(const char* configVarName) {
    ImGui::Text("Secondary Weapon");
    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());

    auto curSelected = (SecondaryWeapon)CONFIGINT(configVarName);
    const char *selected = "None";

    switch (curSelected) {
    case SecondaryWeapon::CZ75Auto: {
        selected = "CZ75 Auto/Tec-9";
        break;
    }
    case SecondaryWeapon::Deagle: {
        selected = "Deagle/Revolver";
        break;
    }
    case SecondaryWeapon::DualBerettas: {
        selected = "Dual Berettas";
        break;
    }
    case SecondaryWeapon::P250: {
        selected = "P250";
        break;
    }
    case SecondaryWeapon::USP: {
        selected = "USP-S/Glock-18";
        break;
    }
    default: {
        break;
    } }

    if (ImGui::BeginCombo("##Secondary Weapon", selected)) {

        if (ImGui::Selectable("None", curSelected == SecondaryWeapon::None, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)SecondaryWeapon::None;

        if (ImGui::Selectable("CZ75 Auto/Tec-9", curSelected == SecondaryWeapon::CZ75Auto, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)SecondaryWeapon::CZ75Auto;

        if (ImGui::Selectable("Deagle/Revolver", curSelected == SecondaryWeapon::Deagle, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)SecondaryWeapon::Deagle;

        if (ImGui::Selectable("Dual Berettas", curSelected == SecondaryWeapon::DualBerettas, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)SecondaryWeapon::DualBerettas;

        if (ImGui::Selectable("P250", curSelected == SecondaryWeapon::P250, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)SecondaryWeapon::P250;

        if (ImGui::Selectable("USP-S/Glock-18", curSelected == SecondaryWeapon::USP, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) = (int)SecondaryWeapon::USP;

        ImGui::EndCombo();
    }
}

void utilitySelectBox(const char* configVarName) {
    ImGui::Text("Utilities");
    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());

    auto curSelected = CONFIGINT(configVarName);

    std::stringstream selected;

    selected << (curSelected & (int)Utilities::Armor ? "Armor, " : "")
             << (curSelected & (int)Utilities::Decoy ? "Decoy, " : "")
             << (curSelected & (int)Utilities::DefuseKit ? "Defuse Kit, " : "")
             << (curSelected & (int)Utilities::Grenade ? "Grenade, " : "")
             << (curSelected & (int)Utilities::Flashbang ? "Flashbang, " : "")
             << (curSelected & (int)Utilities::Molotov ? "Molotov, " : "")
             << (curSelected & (int)Utilities::Smoke ? "Smoke, " : "")
             << (curSelected & (int)Utilities::Taser ? "Taser, " : "");

    if (ImGui::BeginCombo("##Utilities", selected.str().c_str())) {

        if (ImGui::Selectable("Armor", curSelected & (int)Utilities::Armor, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Armor;

        if (ImGui::Selectable("Decoy", curSelected & (int)Utilities::Decoy, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Decoy;

        if (ImGui::Selectable("Defuse Kit", curSelected & (int)Utilities::DefuseKit, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::DefuseKit;

        if (ImGui::Selectable("Grenade", curSelected & (int)Utilities::Grenade, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Grenade;

        if (ImGui::Selectable("Flashbang", curSelected & (int)Utilities::Flashbang, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Flashbang;

        if (ImGui::Selectable("Molotov", curSelected & (int)Utilities::Molotov, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Molotov;

        if (ImGui::Selectable("Smoke", curSelected & (int)Utilities::Smoke, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Smoke;

        if (ImGui::Selectable("Taser", curSelected & (int)Utilities::Taser, ImGuiSelectableFlags_DontClosePopups))
            CONFIGINT(configVarName) ^= (int)Utilities::Taser;


        ImGui::EndCombo();
    }
}

void Menu::drawMiscTab() {
    if (ImGui::BeginTabBar("##miscTabs")) {
        if (ImGui::BeginTabItem("Misc")) {
            ImGui::Columns(2, NULL, false);
            ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.62);
            ImGui::BeginChild("Misc", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.61, ImGui::GetWindowHeight() * 0.64f), true); {
                ImGui::Text("Misc");
                ImGui::Separator();
                ImGui::Checkbox("Developer window", &devWindow);
                ImGui::Checkbox("Disable Watermark", &CONFIGBOOL("Misc>Misc>Misc>Disable Watermark"));
                ImGui::Checkbox("Force square radar", &CONFIGBOOL("Misc>Misc>Misc>Force square radar"));
                ImGui::Checkbox("Rank Revealer", &CONFIGBOOL("Misc>Misc>Misc>Rank Revealer"));

                ImGui::Checkbox("Spectators", &CONFIGBOOL("Misc>Misc>Misc>Spectators"));
                ImGui::Checkbox("Player List", &CONFIGBOOL("Misc>Misc>Misc>Player List"));

                if (CONFIGBOOL("Misc>Misc>Misc>Player List")) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Only When Menu Open", &CONFIGBOOL("Misc>Misc>Misc>Player List Only When Menu Open"));
                }

                ImGui::Checkbox("Flappy Birb", &CONFIGBOOL("Misc>Misc>Misc>Flappy Birb"));
                ImGui::Checkbox("Auto Accept", &CONFIGBOOL("Misc>Misc>Misc>Auto Accept"));
                ImGui::Checkbox("Auto Defuse", &CONFIGBOOL("Misc>Misc>Misc>Auto Defuse"));

                if (CONFIGBOOL("Misc>Misc>Misc>Auto Defuse")) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Silent", &CONFIGBOOL("Misc>Misc>Misc>Silent Defuse"));
                    ImGui::SameLine();
                    ImGui::Checkbox("As late as possible", &CONFIGBOOL("Misc>Misc>Misc>Latest Defuse"));
                }

                ImGui::Checkbox("Chat Filter Bypass", &CONFIGBOOL("Misc>Misc>Misc>Chat Filter Bypass"));

                if (CONFIGBOOL("Misc>Misc>Misc>Use Spam")) {
                    static bool toggled = false;

                    Menu::CustomWidgets::drawKeyBinder("Key", &CONFIGINT("Misc>Misc>Misc>Use Spam Key"), &toggled);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Use Spam", &CONFIGBOOL("Misc>Misc>Misc>Use Spam"));
                ImGui::Checkbox("Disable Setting Cvars", &CONFIGBOOL("Misc>Misc>Misc>Disable Setting Cvars"));
                ImGui::Checkbox("Disable Post Processing", &CONFIGBOOL("Misc>Misc>Misc>Disable Post Processing"));
                ImGui::Checkbox("Disable Panorama Blur", &CONFIGBOOL("Misc>Misc>Misc>Disable Panorama Blur"));
                ImGui::EndChild();
            }

            ImGui::BeginChild("Hitmarkers", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.61, ImGui::GetWindowHeight() * 0.21f), true); {
                ImGui::Text("Hitmarkers");
                ImGui::Separator();
                ImGui::Checkbox("Hitlogs", &CONFIGBOOL("Misc>Misc>Hitmarkers>Hitlogs"));
                ImGui::Checkbox("Hitmarkers", &CONFIGBOOL("Misc>Misc>Hitmarkers>Hitmarkers"));
                ImGui::Checkbox("Hitsound", &CONFIGBOOL("Misc>Misc>Hitmarkers>Hitsound"));
                ImGui::Checkbox("Damage Markers", &CONFIGBOOL("Misc>Misc>Hitmarkers>Damage Markers"));
                ImGui::EndChild();
            }

            ImGui::BeginChild("Auto Buy", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.61, ImGui::GetWindowHeight() * 0.21f), true); {
                ImGui::Text("Auto Buy");
                ImGui::Separator();
                primaryWeaponSelectBox("Misc>Misc>AutoBuy>Primary Weapon");
                secondaryWeaponSelectBox("Misc>Misc>AutoBuy>Secondary Weapon");
                utilitySelectBox("Misc>Misc>AutoBuy>Utilities");
                ImGui::EndChild();
            }

            ImGui::NextColumn();

            ImGui::BeginChild("Config", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.38, ImGui::GetWindowHeight() * 0.307), true); {
                ImGui::Text("Config");
                ImGui::Separator();

                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::ListBoxHeader("##configlist-lbox", ImVec2(0, 100));
                for (std::string file : Config::cfgFiles) {
                    if (ImGui::Button(file.c_str())) {
                        strcpy(Config::configFileName, file.c_str());
                    }
                }
                ImGui::ListBoxFooter();

                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::InputText("##configfile-tbox", Config::configFileName, IM_ARRAYSIZE(Config::configFileName));
                if (ImGui::Button("Save", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.315, 0))) {
                    Config::save();
                }; ImGui::SameLine();
                if (ImGui::Button("Load", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.315, 0))) {
                    Config::load();
                    FULLUPDATE();
                }; ImGui::SameLine();
                if (ImGui::Button("Remove", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.315, 0))) {
                    Config::remove();
                };
                ImGui::EndChild();
            }

            ImGui::BeginChild("Clantag", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.38, ImGui::GetWindowHeight() * 0.21f), true); {
                ImGui::Text("Clantag");
                ImGui::Separator();
                ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                ImGui::InputText("##clantag-tbox", clantag, IM_ARRAYSIZE(clantag));
                ImGui::Checkbox("Clantag", &CONFIGBOOL("Misc>Misc>Clantag>Clantag"));
                ImGui::Checkbox("Marquee", &CONFIGBOOL("Misc>Misc>Clantag>Clantag Marquee"));
                ImGui::Checkbox("Bee Movie Clantag", &CONFIGBOOL("Misc>Misc>Clantag>Bee Movie Clantag"));
                ImGui::EndChild();
            }

            ImGui::BeginChild("Movement", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.38, ImGui::GetWindowHeight() * 0.326f), true); {
                ImGui::Text("Movement");
                ImGui::Separator();
                ImGui::Checkbox("Bhop", &CONFIGBOOL("Misc>Misc>Movement>Bhop"));

                if (CONFIGBOOL("Misc>Misc>Movement>Bhop")) {
                    if (ImGui::Checkbox("Auto Hop", &CONFIGBOOL("Misc>Misc>Movement>Auto Bhop"))) {
                        CONFIGBOOL("Misc>Misc>Movement>Humanised Bhop") = false;
                    }

                    if (ImGui::Checkbox("Humanised Bhop", &CONFIGBOOL("Misc>Misc>Movement>Humanised Bhop"))) {
                        CONFIGBOOL("Misc>Misc>Movement>Auto Bhop") = false;
                    }

                    if (CONFIGBOOL("Misc>Misc>Movement>Humanised Bhop")) {
                        ImGui::Text("Bhop Hitchance");
                        ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                        ImGui::SliderInt("##Bhop Hitchance", &CONFIGINT("Misc>Misc>Movement>Bhop Hitchance"), 0, 100);
                        ImGui::Text("Bhop Max Misses");
                        ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                        ImGui::SliderInt("##Bhop Max Misses", &CONFIGINT("Misc>Misc>Movement>Bhop Max Misses"), 0, 16);
                        ImGui::Text("Bhop Max Hops Hit");
                        ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
                        ImGui::SliderInt("##Bhop Max Hops Hit", &CONFIGINT("Misc>Misc>Movement>Bhop Max Hops Hit"), 0, 16);
                    }
                }

                ImGui::Checkbox("Auto Strafe (BETA)", &CONFIGBOOL("Misc>Misc>Movement>Auto Strafe"));

                if (CONFIGBOOL("Misc>Misc>Movement>Edge Jump")) {
                    static bool toggled = false;
                    Menu::CustomWidgets::drawKeyBinder("Key", &CONFIGINT("Misc>Misc>Movement>Edge Jump Key"), &toggled);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("Edge Jump", &CONFIGBOOL("Misc>Misc>Movement>Edge Jump"));

                if (CONFIGBOOL("Misc>Misc>Movement>JumpBug")) {
                    static bool toggled = false;

                    Menu::CustomWidgets::drawKeyBinder("Key", &CONFIGINT("Misc>Misc>Movement>JumpBug Key"), &toggled);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("JumpBug", &CONFIGBOOL("Misc>Misc>Movement>JumpBug"));

                if (CONFIGBOOL("Misc>Misc>Movement>EdgeBug")) {
                    static bool toggled = false;

                    Menu::CustomWidgets::drawKeyBinder("Key", &CONFIGINT("Misc>Misc>Movement>EdgeBug Key"), &toggled);
                    ImGui::SameLine();
                }

                ImGui::Checkbox("EdgeBug", &CONFIGBOOL("Misc>Misc>Movement>EdgeBug"));
                ImGui::Checkbox("Fast Duck", &CONFIGBOOL("Misc>Misc>Movement>Fast Duck"));
                ImGui::SameLine();
                ImGui::TextDisabled("?");

                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("May cause untrusted, use at own risk!");
                }

                ImGui::EndChild();
            }

            ImGui::BeginChild("Variables", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.61, ImGui::GetWindowHeight() * 0.21f), true); {
                ImGui::Text("Variables");
                ImGui::Separator();
                ImGui::Checkbox("Cheats (sv_cheats)", &CONFIGBOOL("Misc>Misc>Variables>Cheats"));

                if (CONFIGBOOL("Misc>Misc>Variables>Cheats")) {
                    ImGui::Text("Timescale (host_timescale)");
                    ImGui::SliderInt("##Timescale", &CONFIGINT("Misc>Misc>Variables>Timescale"), 1, 8000);
                }
                
                ImGui::Checkbox("Infinite Ammo (sv_infinite_ammo)", &CONFIGBOOL("Misc>Misc>Variables>Infinite Ammo"));
                ImGui::Text("Fake Lag (net_fakelag)");
                ImGui::SliderInt("##Fake Lag", &CONFIGINT("Misc>Misc>Variables>Fake Lag"), 0, 10000);
                ImGui::EndChild();
            }

            ImGui::Columns(1);
            ImGui::TextDisabled("Credits!");

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("sekc (of course)\nAll other contributors on GitHub (xXx-sans-xXx, luk1337, cristeigabriel, crazily, dave (@dweee), keenan, u2ooS, tango1337, nigma1337, vampur, all and any other contributors, and allbombson)\nand ocornut for his great ImGui UI framework");
            
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Skins")) {
            static ItemIndex curWeaponSelected = ItemIndex::WEAPON_AK47;

            if (ImGui::BeginCombo("Weapon", itemIndexMap.at(curWeaponSelected))) {
                for (auto item : itemIndexMap) {
                    if (item.first != ItemIndex::INVALID) {
                        ImGui::PushID(item.second);
                        const bool is_selected = (itemIndexMap.at(curWeaponSelected) == item.second);

                        if (ImGui::Selectable(item.second, is_selected)) {
                            curWeaponSelected = item.first;
                            FULLUPDATE();
                        }

                        ImGui::PopID();
                    }
                }

                ImGui::EndCombo();
            }
            if (curWeaponSelected != ItemIndex::INVALID) {
                char* buf = new char[256];
                snprintf(buf, 256, "Misc>Skins>Skins>%s>PaintKit", itemIndexMap.at(curWeaponSelected));

                if (ImGui::DragInt("PaintKit", &CONFIGINT(buf))) {
                    FULLUPDATE();
                }

                char* buf2 = new char[256];
                snprintf(buf2, 256, "Misc>Skins>Skins>%s>Wear", itemIndexMap.at(curWeaponSelected));

                if (ImGui::SliderInt("Wear", &CONFIGINT(buf2), 0, 100)) {
                    FULLUPDATE();
                }

                char* buf3 = new char[256];
                snprintf(buf3, 256, "Misc>Skins>Skins>%s>StatTrack", itemIndexMap.at(curWeaponSelected));

                if (ImGui::DragInt("StatTrack", &CONFIGINT(buf3))) {
                    FULLUPDATE();
                }
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
