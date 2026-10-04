#include "ParticleRecipe.h"
void CParticleSystem::GetParticleFade(float &a, float &b) const {
    a = definition->contents->entries[47]->value.scalar;
    b = definition->contents->entries[48]->value.scalar;
}
