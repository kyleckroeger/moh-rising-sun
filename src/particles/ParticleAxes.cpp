#include "ParticleRecipe.h"
void CParticleSystem::GetPosition(CVector3 &v) const { v = localToWorld.GetPos(); }
void CParticleSystem::GetRightward(CVector3 &v) const { v = localToWorld.GetRight(); }
void CParticleSystem::GetForward(CVector3 &v) const { v = localToWorld.GetFront(); }
void CParticleSystem::GetUpward(CVector3 &v) const { v = localToWorld.GetUp(); }
