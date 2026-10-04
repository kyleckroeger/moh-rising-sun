#include "ParticleRecipe.h"
// Normalize the no-fog field, then invert the resulting integer flag.
int CParticleSystem::GetFogEnable() const {
    int disabled;
    if (definition->contents->entries[46]->value.integer == 0)
        disabled = 0;
    else
        disabled = 1;
    return disabled ^ 1;
}
