#include "ParticleRecipe.h"
void CProcParticleDef::SetVelMin(float x, float y, float z) {
    contents->entries[13]->value.scalar = x;
    contents->entries[14]->value.scalar = y;
    contents->entries[15]->value.scalar = z;
    if (!(contents->entries[16]->value.scalar >= x))
        contents->entries[16]->value.scalar = x;
    if (!(contents->entries[17]->value.scalar >= y))
        contents->entries[17]->value.scalar = y;
    if (!(contents->entries[18]->value.scalar >= z))
        contents->entries[18]->value.scalar = z;
}
