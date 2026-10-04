#include "ParticleRecipe.h"
void CParticleSystem::GetParticlePosition(CVector3 &a, CVector3 &b) const {
    SetVectorPrefix(a, definition->contents->entries[7]->value.scalar,
                    definition->contents->entries[8]->value.scalar,
                    definition->contents->entries[9]->value.scalar);
    SetVectorPrefix(b, definition->contents->entries[10]->value.scalar,
                    definition->contents->entries[11]->value.scalar,
                    definition->contents->entries[12]->value.scalar);
}
