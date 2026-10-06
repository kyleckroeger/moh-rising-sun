#include "ParticleRecipe.h"
// Clear the unidentified top flag bit only for nondestroyable systems.
void CParticleSystem::Terminate() {
    if (!flags.destroyable)
        flags.unknown_31 = 0;
    MarkForDestruction(1);
}
