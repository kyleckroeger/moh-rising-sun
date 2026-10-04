#include "ParticleRecipe.h"
void CParticleSystem::GetParticleVelocity(CVector3 &a, CVector3 &b) const {
    SetVectorPrefix(a, definition->contents->entries[13]->value.scalar,
                    definition->contents->entries[14]->value.scalar,
                    definition->contents->entries[15]->value.scalar);
    SetVectorPrefix(b, definition->contents->entries[16]->value.scalar,
                    definition->contents->entries[17]->value.scalar,
                    definition->contents->entries[18]->value.scalar);
}
