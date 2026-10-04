#include "ParticleRecipe.h"
int CParticleSystem::GetRenderType() const { return definition->contents->entries[30]->value.integer; }
int CParticleSystem::GetEmmisionRate() const { return definition->contents->entries[3]->value.integer; }
int CParticleSystem::GetEmmisionDelay() const { return definition->contents->entries[4]->value.integer; }
float CParticleSystem::GetSystemLifetime() const { return definition->contents->entries[5]->value.scalar; }
float CParticleSystem::GetParticleLifetime() const { return definition->contents->entries[36]->value.scalar; }
void CParticleSystem::GetParticleRotation(float &a, float &b) const {
    a = definition->contents->entries[37]->value.scalar;
    b = definition->contents->entries[38]->value.scalar;
}
