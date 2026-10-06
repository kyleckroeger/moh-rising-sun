#include "ParticleRecipe.h"
void *CParticleSystem::operator new(unsigned int, void *storage) { return storage; }
void CParticleSystem::operator delete(void *) {}
