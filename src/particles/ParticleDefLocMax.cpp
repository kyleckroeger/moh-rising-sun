#include "ParticleRecipe.h"
void CProcParticleDef::SetLocMax(float x, float y, float z) {
    contents->entries[10]->value.scalar = x;
    contents->entries[11]->value.scalar = y;
    contents->entries[12]->value.scalar = z;
    if (!(contents->entries[7]->value.scalar <= x))
        contents->entries[7]->value.scalar = x;
    if (!(contents->entries[8]->value.scalar <= y))
        contents->entries[8]->value.scalar = y;
    if (!(contents->entries[9]->value.scalar <= z))
        contents->entries[9]->value.scalar = z;
}
