#include "ParticleRecipe.h"
void CParticleSystem::GetParticleSize(CVector3 &a, CVector3 &b, float &extra) const {
    SetVectorPrefix(a, definition->contents->entries[31]->value.scalar,
                    definition->contents->entries[33]->value.scalar, 0.0f);
    SetVectorPrefix(b, definition->contents->entries[32]->value.scalar,
                    definition->contents->entries[34]->value.scalar, 0.0f);
    extra = definition->contents->entries[35]->value.scalar;
}
