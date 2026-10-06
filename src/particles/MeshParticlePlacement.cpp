#include "ParticleRecipe.h"
void *MeshParticleSystem::operator new(unsigned int, void *storage) { return storage; }
void MeshParticleSystem::operator delete(void *) {}
