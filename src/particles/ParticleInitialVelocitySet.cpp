#include <math.h>
#include "ParticleRecipe.h"
// Clear flag bit 29, then mark the system moving when either the initial velocity
// or the acceleration has a squared length of at least 1e-6. The two comparison
// spellings reproduce the original branch forms.
void CParticleSystem::SetSystemInitialVelocity(const CVector3 &v) {
    initialVelocity = v;
    flags.unknown_29 = 0;
    flags.moving = fabsf(initialVelocity.Dot(initialVelocity)) >= 1e-6f ||
                   !(fabsf(acceleration.Dot(acceleration)) < 1e-6f);
}
