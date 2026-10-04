#include "ParticleRecipe.h"
bool CProcParticleDef::IsValid() { return contents != 0; }
void CProcParticleDef::SetEmitDelay(int x) {
    if (x <= 0)
        x = 1;
    contents->entries[4]->value.integer = x;
}
void CProcParticleDef::SetEmitRate(int x) {
    if (x <= 0)
        x = 1;
    contents->entries[3]->value.integer = x;
}
