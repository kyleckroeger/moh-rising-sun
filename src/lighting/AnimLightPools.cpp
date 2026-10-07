#include "Light.h"
// Unlink from the used list without a membership check, then push onto the free
// list; the same shape as CParticleSystem::ReleaseSystem.
static inline void ReleaseToPool(AnimLightPool *pool, CPropertyAnimLight *light) {
    if (light == pool->active) {
        pool->active = light->next;
    } else {
        for (CPropertyAnimLight *p = pool->active; p; p = p->next) {
            if (p->next == light) {
                p->next = light->next;
                break;
            }
        }
    }
    pool->count--;
    light->next = pool->free;
    pool->free = light;
}
void CAnimLightManager::Destroy(CInstancedAnimLight *light) { ReleaseToPool(&instancedLights, light); }
void CAnimLightManager::Destroy(CPropertyAnimLight *light) { ReleaseToPool(&propertyLights, light); }
