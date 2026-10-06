#include "Light.h"
// Original light.cpp storage remains external context.
extern CAnimLightManager g_AnimLightManager;
CPropertyAnimLight::~CPropertyAnimLight() {}
void CPropertyAnimLight::InitFromProperty() {}
void CPropertyAnimLight::Destroy() {
    CLight::Destroy();
    g_AnimLightManager.Destroy(this);
}
