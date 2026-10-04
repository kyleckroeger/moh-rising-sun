#include "ParticleRecipe.h"
void CProcParticleDef::SetPartLifetime(float x) {
    if (!(x >= 0.0f))
        x = 0.0f;
    contents->entries[36]->value.scalar = x;
}
