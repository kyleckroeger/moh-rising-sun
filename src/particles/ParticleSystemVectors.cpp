#include "ParticleRecipe.h"
void CParticleSystem::GetSystemInitialVelocity(CVector3 &v) const { v = initialVelocity; }
void CParticleSystem::GetSystemAcceleration(CVector3 &v) const { v = acceleration; }
