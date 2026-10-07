#include "Light.h"
// Original global default volume; data remains external context.
extern BPDLightVolume g_DefaultLightVolume;
BPDLightVolume *CLight::GetDefaultLightVolume() { return &g_DefaultLightVolume; }
