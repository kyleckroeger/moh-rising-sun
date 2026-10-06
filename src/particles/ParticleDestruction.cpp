#include "ParticleRecipe.h"
void CParticleSystem::MarkForDestruction(int countdown) {
    if (flags.destroyable) {
        ISubject::MarkForDestruction(countdown);
        g_scene.Remove(*this);
    }
}
void CParticleSystem::Destroy() {
    if (flags.destroyable) {
        ReleaseSystem(this);
        delete this;
    }
}
