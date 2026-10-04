#include "ParticleRecipe.h"
void CParticleSystem::GetParticleAlpha(float &a, float &b, float &c) const {
    a = definition->contents->entries[25]->value.integer;
    b = definition->contents->entries[45]->value.integer;
    c = definition->contents->entries[29]->value.integer;
}
