#include "ParticleRecipe.h"

// Move the first free system to the head of the used list.
static inline CParticleSystem *PopFree() {
    CParticleSystem *system = g_particleSystemList.free;
    g_particleSystemList.free = system->next;
    system->next = g_particleSystemList.active;
    g_particleSystemList.active = system;
    g_particleSystemList.count++;
    return system;
}
// Positive priorities may take any free system; zero requires more than nine
// unused slots; negative priorities never allocate.
CParticleSystem *CParticleSystem::AllocSystem(int priority) {
    CParticleSystem *system = 0;
    int available = g_particleSystemList.capacity - g_particleSystemList.count;
    if (priority > 0 && available >= 0) {
        if (g_particleSystemList.free)
            system = PopFree();
    } else if (priority >= 0 && available > 9) {
        if (!g_particleSystemList.free)
            system = 0;
        else
            system = PopFree();
    }
    return system;
}
// Unlink from the used list without a membership check, then push onto the free list.
void CParticleSystem::ReleaseSystem(CParticleSystem *system) {
    ParticleSystemList *list = &g_particleSystemList;
    if (system == list->active) {
        list->active = system->next;
    } else {
        for (CParticleSystem *p = list->active; p; p = p->next) {
            if (p->next == system) {
                p->next = system->next;
                break;
            }
        }
    }
    list->count--;
    system->next = list->free;
    list->free = system;
}
