#include "ParticleRecipe.h"

// Original particlesystem.cpp storage remains external context.
extern unsigned int g_CurrentSeed;
// One linear-congruential step of the shared particle seed.
static inline void AdvanceSeed() { g_CurrentSeed = g_CurrentSeed * 1103515245 + 12345; }
// Each counted particle consumes eleven steps.
unsigned int IncrementSeed(unsigned int seed, int count) {
    g_CurrentSeed = seed;
    for (int i = 0; i < count; i++) {
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
        AdvanceSeed();
    }
    return g_CurrentSeed;
}
