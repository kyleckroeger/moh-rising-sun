#include "Light.h"
// Original light.cpp storage remains external context.
extern CAnimLightManager g_AnimLightManager;
CInstancedAnimLight::CInstancedAnimLight(MOH_animatedLight_Struct *light) : CPropertyAnimLight(light) {}
void CInstancedAnimLight::Destroy() {
    CLight::Destroy();
    g_AnimLightManager.Destroy(this);
}
