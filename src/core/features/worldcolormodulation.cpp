#include "features.hpp"

void Features::ColorModulation::updateColorModulation() {
    for (auto i = Interfaces::materialSystem->FirstMaterial(); i != Interfaces::materialSystem->InvalidMaterial(); i = Interfaces::materialSystem->NextMaterial(i)) {
        auto material = Interfaces::materialSystem->GetMaterial(i);

        if (material == nullptr) {
            continue;
        }

        if (strstr(material->getTextureGroupName(), "World")) {
            material->setColor(CONFIGCOL("Visuals>World>World>World Color Modulation"));
        }

        if (strstr(material->getTextureGroupName(), "SkyBox")) {
            material->setColor(CONFIGCOL("Visuals>World>World>SkyBox Color Modulation"));
        }
    }
}
