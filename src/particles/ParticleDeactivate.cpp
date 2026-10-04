#include "ParticleRecipe.h"
#include "ParticleClock.h"
void CParticleSystem::DeActivate() {
    flags.active = 0;
    deactivationTicks = CPSManager::GetCurrentTicks();
}
