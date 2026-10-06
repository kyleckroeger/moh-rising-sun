#include "Light.h"
int CLight::GetPropertyID() { return 0; }
CLight *CLight::AsLight() { return this; }
const CLight *CLight::AsLight() const { return this; }
int CPropertyAnimLight::GetPropertyID() { return record->core.field38; }
