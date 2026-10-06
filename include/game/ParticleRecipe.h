#ifndef GAME_PARTICLE_RECIPE_H
#define GAME_PARTICLE_RECIPE_H
#pragma interface
#include "CMatrix.h"
#include "SceneNode.h"

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
// Bit positions are established by the matching accessors; names are descriptive.
// This view relies on the target compiler's big-endian bitfield layout.
struct ParticleFlags {
    unsigned int unknown_31 : 1;
    unsigned int active : 1;
    unsigned int unknown_29 : 1;
    unsigned int moving : 1;
    unsigned int rotating : 1;
    unsigned int destroyable : 1;
    unsigned int dying : 1;
    unsigned int fade : 1;
    unsigned int eternal : 1;
    unsigned int profile : 1;
    unsigned int unknown_low22 : 22;
};
// Only a packed four-byte color prefix is accessed; full CColor remains opaque.
class CColor;
class CParticleDef;
class CParticleSystem;
class MeshParticleSystem;
// Virtual slots 89-107, in the order of the original IParticleSystem table at
// 0x802e2048. Return types repeat the accepted overrides' scoped choices or are
// void placeholders; data members are not declared.
class IParticleSystem : public IMovingSceneNode {
  public:
    virtual void Init(CParticleDef *);
    virtual void Reload();
    virtual void DeActivate();
    virtual void Terminate();
    virtual void Stop();
    virtual void Start();
    virtual unsigned int IsEternal() const;
    virtual unsigned int IsActive() const;
    virtual unsigned int IsDying() const;
    virtual unsigned int IsMoving() const;
    virtual unsigned int IsRotating() const;
    virtual unsigned int IsDestroyable() const;
    virtual void SetDestroyable(bool);
    virtual CProcParticleDef *GetDef() const;
    virtual void Profile(bool);
    virtual CParticleSystem *AsProceduralParticleSystem();
    virtual const CParticleSystem *AsProceduralParticleSystem() const;
    virtual MeshParticleSystem *AsMeshParticleSystem();
    virtual const MeshParticleSystem *AsMeshParticleSystem() const;
};
// Scoped view through the pool link. Storage after the 24-byte IObserver prefix
// and before 0x90 belongs to unknown members of this class or its bases.
// UseFade is the only virtual slot (108) this class adds.
class CParticleSystem : public IParticleSystem {
  public:
    unsigned char unknown_18[0x90 - 0x18];
    CMatrix localToWorld;
    unsigned char unknown_d0[0x70];
    CProcParticleDef *definition;
    unsigned char unknown_144[12];
    float deactivationTicks;
    unsigned char unknown_154[12];
    unsigned int seed;
    unsigned char unknown_164[12];
    ParticleFlags flags;
    unsigned char unknown_174[12];
    CParticleSystem *next;
    virtual ~CParticleSystem();
    virtual void MarkForDestruction(int);
    virtual void Destroy();
    static CParticleSystem *AllocSystem(int);
    static void ReleaseSystem(CParticleSystem *);
    void *operator new(unsigned int, void *);
    void operator delete(void *);
    void Start();
    void DeActivate();
    void Terminate();
    void Stop();
    unsigned int IsEternal() const;
    unsigned int IsActive() const;
    unsigned int IsDying() const;
    unsigned int IsMoving() const;
    unsigned int IsRotating() const;
    unsigned int IsDestroyable() const;
    virtual unsigned int UseFade() const;
    void SetDestroyable(bool);
    CProcParticleDef *GetDef() const;
    void Profile(bool);
    void GetLocalToWorld(CMatrix &) const;
    void SetLocalToWorld(const CMatrix &);
    void GetTMLocalToWorld(CMatrix &) const;
    void SetTMLocalToWorld(const CMatrix &);
    void Orthonormalize();
    void GetParticleAlpha(float &, float &, float &) const;
    void GetParticleColor(CColor &, CColor &, CColor &, CColor &) const;
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
    unsigned int IsDrawEnabled() const;
    IMovingSceneNode *AsMovingNode();
    const IMovingSceneNode *AsMovingNode() const;
    CParticleSystem *AsProceduralParticleSystem();
    const CParticleSystem *AsProceduralParticleSystem() const;
    void Reset();
    void Halt();
};
// Member-only view; MeshParticleSystem storage and inheritance remain unknown.
class MeshParticleSystem {
  public:
    void *operator new(unsigned int, void *);
    void operator delete(void *);
};
// The original 20-byte global particle-system pool: a storage block threaded
// through the link at +0x180 into used and free lists. Member names are descriptive.
struct ParticleSystemList {
    CParticleSystem *storage;
    CParticleSystem *active;
    CParticleSystem *free;
    int capacity;
    int count;
};
extern ParticleSystemList g_particleSystemList;
// Free function; the shared seed it advances is original external storage.
unsigned int IncrementSeed(unsigned int, int);
// Write only the independently established three-float CVector3 prefix.
inline void SetVectorPrefix(CVector3 &result, float x, float y, float z) {
    float *p = reinterpret_cast<float *>(&result);
    p[0] = x;
    p[1] = y;
    p[2] = z;
}
#endif
