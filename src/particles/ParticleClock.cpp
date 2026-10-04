#include "ParticleClock.h"

// Original particlesystemmanager.cpp storage remains external context.
extern float g_numCurrentTicks;
void CPSManager::Update(float elapsed) { g_numCurrentTicks += elapsed; }
float CPSManager::GetCurrentTicks() { return g_numCurrentTicks; }
