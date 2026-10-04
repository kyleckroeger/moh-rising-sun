#include "ParticleRecipe.h"
void CParticleSystem::Start() { flags.active = 1; }
unsigned int CParticleSystem::IsEternal() const { return flags.eternal; }
unsigned int CParticleSystem::IsActive() const { return flags.active; }
unsigned int CParticleSystem::IsDying() const { return flags.dying; }
unsigned int CParticleSystem::IsMoving() const { return flags.moving; }
unsigned int CParticleSystem::IsRotating() const { return flags.rotating; }
unsigned int CParticleSystem::IsDestroyable() const { return flags.destroyable; }
unsigned int CParticleSystem::UseFade() const { return flags.fade; }
void CParticleSystem::SetDestroyable(bool enabled) { flags.destroyable = enabled; }
CProcParticleDef *CParticleSystem::GetDef() const { return definition; }
void CParticleSystem::Profile(bool enabled) { flags.profile = enabled; }
