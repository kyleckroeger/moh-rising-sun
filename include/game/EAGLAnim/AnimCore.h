// Scoped reconstructed interface of the EAGLAnim base declarations used by the animation fragments.
//
// Evidence policy: class names, method names and signatures come from the
// original GR8E69 mangled symbols (namespace EAGLAnim). Member offsets come from
// the original FnDeltaF1 instructions. Virtual-slot order comes from the original
// `_vt.Q28EAGLAnim9FnDeltaF1` (0x802ecb78) and `_vt.Q28EAGLAnim17MemoryPoolManager`
// (0x802ec688) tables. Member *names* that have no original symbol are taken from
// the attributed public NFS Most Wanted EAGL4Anim reference (dbalatoni13/nfsmw,
// CC0; see docs/Animation.md) as research names, and are marked below. Nothing here
// claims the complete original class declarations; only what this unit needs.
#ifndef MOH_EAGLANIM_ANIMCORE_H
#define MOH_EAGLANIM_ANIMCORE_H
#pragma interface
#include "AttributeSupport.h"

namespace EAGLInternal {
extern void *(*EAGLMalloc)(unsigned int, const char *);
// Original symbol `_12EAGLInternal.EAGLFree` (0x802d2e0c), a 4-byte pointer
// holding DefaultFree__12EAGLInternalPvUi; called as (*EAGLFree)(ptr, size).
extern void (*EAGLFree)(void *, unsigned int);
} // namespace EAGLInternal

namespace EAGLAnim {

// Alignment helpers observed inline: (x + 1) & ~1 and (x + 3) & ~3.
// Names follow the reference AnimUtil.h; no original symbol exists.
static inline int AlignSize2(int x) { return (x + 1) & ~1; }
static inline int AlignSize4(int x) { return (x + 3) & ~3; }
// Observed as fctiwz; reference name FloatToInt.
static inline int FloatToInt(float f) { return (int)f; }

// Two-byte type id prefix of every AnimMemoryMap (DeltaF1 fields start at +4).
// ANIM_DELTAF1 = 21 is verified by its constructor (stored to FnAnim::mType).
// DELTAF3 = 20 is independently stored by its original constructor.
class AnimTypeId {
  public:
    enum Type {
        ANIM_COMPOUND = 15,
        ANIM_DELTAF3 = 20,
        ANIM_DELTAF1 = 21,
        ANIM_STATELESSQ = 22,
        ANIM_STATELESSF3 = 23,
        ANIM_POSEANIM = 25
    };
    AnimTypeId() {}
    AnimTypeId(const AnimTypeId &other) : mId(other.mId) {}
    ~AnimTypeId() {}
    Type GetType() const { return static_cast<Type>(mId); }

  private:
    unsigned short mId; // reference name
};

// Four-byte header shared by memory-mapped animations (observed: DeltaF1 data
// begins at offset 4). Field names are reference names.
class AnimMemoryMap {
  public:
    unsigned short GetTargetCheckSum() const { return mTargetCheckSum; }
    AnimTypeId GetType() const { return mAnimTypeId; }

  protected:
    AnimTypeId mAnimTypeId;
    unsigned short mTargetCheckSum;
};

// Original symbols: BoneMask::SetBone(int, bool) (0x801ede64).
// GetBone is observed inline: (mMask[i >> 5] & (1 << (i & 31))) != 0.
// The original BoneMask copy constructor at 0x801eddc8 copies exactly 32 bytes.
class BoneMask {
  public:
    BoneMask(bool);
    BoneMask(const BoneMask &);
    BoneMask operator~() const;
    bool operator==(const BoneMask &) const;
    bool GetBone(int boneIdx) const { return (mMask[boneIdx >> 5] & (1 << (boneIdx & 0x1F))) != 0; }
    void SetBone(int boneIdx, bool on);
    void SetAll(bool);

  private:
    unsigned int mMask[8];
};

class AnimStat;
class MatchPhaseInput;
class PhaseValue;
class PhaseChan;
class EventHandler;
class PosePaletteBank;
class StateTest;
class AttributeBlock;
struct State;

// Original symbols: _vt.Q28EAGLAnim11FnAnimSuper (24 bytes: one virtual), _._Q28EAGLAnim11FnAnimSuper.
struct FnAnimSuper {
    FnAnimSuper() {}
    virtual ~FnAnimSuper() {}
};

// Virtual order verified from the FnDeltaF1 vtable. Default bodies are
// reconstructed in src/eagl_anim/FnAnim.cpp (0x801ee140..0x801ee1bf).
// Offsets: vptr 0, unknown word 4 (zeroed by the base constructor), mType 8.
class FnAnim : public FnAnimSuper {
  public:
    FnAnim() : unknown_4(0) {}
    AnimTypeId::Type GetType() const { return mType; }
    virtual unsigned short GetTargetCheckSum() const;
    virtual void UseFPS(bool);
    virtual void Eval(float previousTime, float currentTime, float *dofs);
    virtual bool GetLength(float &timeLength) const;
    virtual bool FindMatchTime(const MatchPhaseInput &, float &) const;
    virtual bool EvalSQT(float currentTime, float *sqt, const BoneMask *boneMask);
    virtual bool EvalPhase(float, PhaseValue &);
    virtual bool EvalVel2D(float currentTime, float *velocity);
    virtual bool EvalEvent(float, float, EventHandler **, void *);
    virtual bool EvalWeights(float currentTime, float *weights);
    virtual bool EvalState(float, State *);
    virtual bool EvalPose(float, const PosePaletteBank *, float *);
    virtual bool FindTime(const StateTest &, float, float &);
    virtual const PhaseChan *GetPhaseChan();
    virtual const AttributeBlock *GetAttributes() const;
    template <typename T> bool GetAttribute(AttributeId id, T &result) const {
        const AttributeBlock *attribBlock = GetAttributes();
        if (attribBlock)
            return attribBlock->GetAttribute(id, result);
        return false;
    }

    virtual bool GetAnimatedBones(BoneMask *, int &, int &, int &);
    virtual bool GetAnimatedBonesAux(BoneMask *, int &, int &, int &);

    // Original symbol _Q28EAGLAnim6FnAnim.gReverseDeltaSumEnabled: 4 bytes, value 1.
    static bool IsReverseDeltaSumEnabled() { return gReverseDeltaSumEnabled; }

    unsigned int unknown_4;

  protected:
    static bool gReverseDeltaSumEnabled;
    AnimTypeId::Type mType;
};

// Original symbols: __Q28EAGLAnim15FnAnimMemoryMap (ctor, 0x801ee1c0),
// _._Q28EAGLAnim15FnAnimMemoryMap (dtor, 0x801ee1e0).
// mpAnim at offset 12 (reference name).
class FnAnimMemoryMap : public FnAnim {
  public:
    FnAnimMemoryMap();
    virtual ~FnAnimMemoryMap();
    virtual void SetAnimMemoryMap(AnimMemoryMap *anim);
    virtual AnimMemoryMap *GetAnimMemoryMap();
    virtual const AnimMemoryMap *GetAnimMemoryMap() const;
    // Override observed in the FnDeltaF1 vtable (slot +16 = 0x801ee22c).
    virtual unsigned short GetTargetCheckSum() const;

  protected:
    AnimMemoryMap *mpAnim;
};

// Original symbols: _Q28EAGLAnim17MemoryPoolManager.gMemoryManager (0x802d2e5c)
// and the *Aux virtual methods. Slot order verified from _vt.Q28EAGLAnim17MemoryPoolManager.
// FnDeltaF1 calls NewBlockAux (slot +48) and DeleteBlockAux (slot +56) through gMemoryManager.
class MemoryPoolManager {
  public:
#include "AnimAllocation.h"
    virtual ~MemoryPoolManager();
    virtual void InitAux(unsigned int poolSize);
    virtual void CleanupAux();
    virtual unsigned int GetMemoryPoolUsageAux();
    virtual unsigned int GetFreePoolSizeAux();
    virtual void *NewBlockAux(unsigned int size);
    virtual void DeleteBlockAux(void *ptr);
    virtual void *NewBlockByIdxAux(unsigned short idx);
    virtual void DeleteBlockByIdxAux(unsigned short idx, void *ptr);
    virtual FnAnim *NewFnAnimAux(AnimTypeId::Type type);
    virtual FnAnimMemoryMap *NewFnAnimAux(AnimMemoryMap *anim);
    virtual void DeleteFnAnimAux(FnAnim *fnAnim);
    virtual void InitAnimMemoryMapAux(AnimMemoryMap *memMap);
    virtual void ResetPoolAux();

    static FnAnim *NewFnAnim(AnimTypeId::Type t) { return gMemoryManager->NewFnAnimAux(t); }
    static void *NewBlockByIdx(unsigned short idx) { return gMemoryManager->NewBlockByIdxAux(idx); }
    static unsigned int GetMemoryPoolUsage() { return gMemoryManager->GetMemoryPoolUsageAux(); }
    static void ResetPool() { gMemoryManager->ResetPoolAux(); }
    static void DeleteBlockByIdx(unsigned short idx, void *p) { gMemoryManager->DeleteBlockByIdxAux(idx, p); }
    static void *NewBlock(unsigned int size) { return gMemoryManager->NewBlockAux(size); }
    static void DeleteBlock(void *p) { gMemoryManager->DeleteBlockAux(p); }
    static FnAnimMemoryMap *NewFnAnim(AnimMemoryMap *a) { return gMemoryManager->NewFnAnimAux(a); }
    static void DeleteFnAnim(FnAnim *a) { gMemoryManager->DeleteFnAnimAux(a); }
    static void InitAnimMemoryMap(AnimMemoryMap *a) { gMemoryManager->InitAnimMemoryMapAux(a); }

  protected:
    static MemoryPoolManager *gMemoryManager;
    static char *gMemoryPool;
    static char *gMemoryPoolFree;
    static unsigned int gMemoryPoolSize;
    static unsigned short gMaxIdx;
    static unsigned short gFreeListSize[25];
    static char *gFreeList[26];
    static char *gSizeFreeList[256];
};

} // namespace EAGLAnim

#endif
