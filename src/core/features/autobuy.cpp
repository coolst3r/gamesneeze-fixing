#include "../../includes.hpp"
#include "features.hpp"

void Features::AutoBuy::event(IGameEvent* event) {
	if (strstr(event->GetName(), "round_start") == nullptr) {
        return;
    }

    std::stringstream buy;
            
    auto primary = (PrimaryWeapon)CONFIGINT("Misc>Misc>AutoBuy>Primary Weapon");
    auto secondary = (SecondaryWeapon)CONFIGINT("Misc>Misc>AutoBuy>Secondary Weapon");
    auto utilities = CONFIGINT("Misc>Misc>AutoBuy>Utilities");

    switch (primary) {
    case PrimaryWeapon::AUG: {
        buy << "buy aug;";
        break;
    }
    case PrimaryWeapon::AWP: {
        buy << "buy awp;";
        break;
    }
    case PrimaryWeapon::FAMAS: {
        buy << "buy famas;";
        break;
    }
    case PrimaryWeapon::M249: {
        buy << "buy m249;";
        break;
    }
    case PrimaryWeapon::M4A4: {
        buy << "buy m4a1;";
        break;
    }
    case PrimaryWeapon::MAG7: {
        buy << "buy mag7;";
        break;
    }
    case PrimaryWeapon::MP7: {
        buy << "buy mp7;";
        break;
    }
    case PrimaryWeapon::MP9: {
        buy << "buy mp9;";
        break;
    }
    case PrimaryWeapon::Negev: {
        buy << "buy negev;";
        break;
    }
    case PrimaryWeapon::Nova: {
        buy << "buy nova;";
        break;
    }
    case PrimaryWeapon::P90: {
        buy << "buy p90;";
        break;
    }
    case PrimaryWeapon::PPBizon: {
        buy << "buy bizon;";
        break;
    }
    case PrimaryWeapon::SCAR20: {
        buy << "buy scar20;";
        break;
    }
    case PrimaryWeapon::SSG08: {
        buy << "buy ssg08;";
        break;
    }
    case PrimaryWeapon::UMP45: {
        buy << "buy ump45;";
        break;
    }
    case PrimaryWeapon::XM1014: {
        buy << "buy xm1014;";
        break;
    }
    default: {
        break;
    } }

    switch (secondary) {
    case SecondaryWeapon::CZ75Auto: {
        buy << "buy fn57;";
        break;
    }
    case SecondaryWeapon::Deagle: {
        buy << "buy deagle;";
        break;
    }
    case SecondaryWeapon::DualBerettas: {
        buy << "buy elite;";
        break;
    }
    case SecondaryWeapon::P250: {
        buy << "buy p250;";
        break;
    }
    case SecondaryWeapon::USP: {
        buy << "buy usp_silencer;";
        break;
    }
    default: {
        break;
    } }

    buy << (utilities & (int)Utilities::Decoy ? "buy decoy;" : "")
        << (utilities & (int)Utilities::DefuseKit ? "buy defuser;" : "")
        << (utilities & (int)Utilities::Grenade ? "buy hegrenade;" : "")
        << (utilities & (int)Utilities::Flashbang ? "buy flashbang;" : "")
        << (utilities & (int)Utilities::Molotov ? "buy incgrenade;" : "")
        << (utilities & (int)Utilities::Smoke ? "buy smokegrenade;" : "")
        << (utilities & (int)Utilities::Armor ? "buy vesthelm;" : "")
        << (utilities & (int)Utilities::Taser ? "buy taser;" : "");

    auto str = buy.str();

    if (!str.empty()) {
    	Interfaces::engine->ExecuteClientCmdUnrestricted(str.c_str());
    }
}
