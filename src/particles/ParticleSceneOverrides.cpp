#include "ParticleRecipe.h"
unsigned int CParticleSystem::IsDrawEnabled() const { return 1; }
IMovingSceneNode *CParticleSystem::AsMovingNode() { return this; }
const IMovingSceneNode *CParticleSystem::AsMovingNode() const { return this; }
void CParticleSystem::Reset() {}
void CParticleSystem::Halt() {}
