#ifndef GAME_PARTICLE_CLOCK_H
#define GAME_PARTICLE_CLOCK_H

// Scoped static interface; no complete CPSManager storage declaration.
class CPSManager {
  public:
    static void Update(float);
    static float GetCurrentTicks();
};

#endif
