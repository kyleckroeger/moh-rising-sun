#ifndef GAME_PARTICLE_RECIPE_H
#define GAME_PARTICLE_RECIPE_H
#include "CMatrix.h"

// Scoped GameCube storage views established by the recipe getters/setters.
// Names of data members and helper types are descriptive. These are prefixes,
// not complete class declarations or allocation sizes; see docs/ParticleRecipes.md.
// A recipe-entry prefix: the value at +8 is accessed as an int or float.
struct ParticleRecipeValue {
    unsigned int unknown_0, unknown_4;
    union {
        int integer;
        float scalar;
    } value;
};
struct LevelFileContentsStruct_ {
    unsigned int unknown_0, unknown_4;
    ParticleRecipeValue **entries;
};
class CProcParticleDef {
  public:
    unsigned int unknown_0;
    LevelFileContentsStruct_ *contents;
    bool IsValid();
    void SetEmitDelay(int);
    void SetEmitRate(int);
    void SetPartLifetime(float);
    void SetLocMin(float, float, float);
    void SetLocMax(float, float, float);
    void SetVelMin(float, float, float);
    void SetVelMax(float, float, float);
};
// Only accesses at +0x140 and +0x160 are modeled; inherited state stays unknown.
class CParticleSystem {
  public:
    unsigned char unknown_0[0x140];
    CProcParticleDef *definition;
    unsigned char unknown_144[0x1c];
    unsigned int seed;
    unsigned int GetSeed() const;
    void SetSeed(unsigned int);
    int GetRenderType() const;
    int GetEmmisionRate() const;
    int GetEmmisionDelay() const;
    float GetSystemLifetime() const;
    float GetParticleLifetime() const;
    void GetParticleRotation(float &, float &) const;
    void GetParticleFade(float &, float &) const;
    void GetParticlePosition(CVector3 &, CVector3 &) const;
    void GetParticleVelocity(CVector3 &, CVector3 &) const;
    void GetParticleSize(CVector3 &, CVector3 &, float &) const;
    int GetFogEnable() const;
    void SetParticleLifetime(float);
};
// Write only the independently established three-float CVector3 prefix.
inline void SetVectorPrefix(CVector3 &result, float x, float y, float z) {
    float *p = reinterpret_cast<float *>(&result);
    p[0] = x;
    p[1] = y;
    p[2] = z;
}
#endif
