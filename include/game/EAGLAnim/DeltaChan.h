// Adapted from the CC0 NFS Most Wanted reconstruction; see docs/Animation.md.
// Names of members are descriptive reference names. Only the accessed storage
// and original method interfaces needed by these fragments are represented.
#ifndef MOH_EAGLANIM_DELTACHAN_H
#define MOH_EAGLANIM_DELTACHAN_H
#pragma interface

#include "DeltaCompressedData.h"
#include "AnimCore.h"

namespace EAGLAnim {

// The variable index array starts at +10, immediately after the frame count.
// sizeof(DeltaChan) includes alignment padding and is not its serialized size.
class DeltaChan : public AnimMemoryMap {
  public:
    unsigned short GetNumFrames() const { return mNumFrames; }
    int GetNumDofs() const { return mData->GetNumDofs(); }
    unsigned short *GetDofIndices() { return reinterpret_cast<unsigned short *>(&mNumFrames + 1); }
    const unsigned short *GetDofIndices() const {
        return reinterpret_cast<const unsigned short *>(&mNumFrames + 1);
    }
    DeltaCompressedData *GetDeltaData() const { return mData; }

  private:
    DeltaCompressedData *mData; // +4
    unsigned short mNumFrames;  // +8
};

// Constructor accesses and deleting destructor establish a 36-byte object.
class FnDeltaChan : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    FnDeltaChan();
    virtual bool GetLength(float &timeLength) const;
    virtual ~FnDeltaChan();
    virtual void SetAnimMemoryMap(AnimMemoryMap *anim);
    void EvalToPrevValues(int frame);
    void EvalToPrevValues(int frame, int numDofPerBone, int, const unsigned short *);

  protected:
    int mPrevFrame;            // +16
    float *mPrevValues;        // +20
    int mNumDofs;              // +24
    unsigned short *mDofMask;  // +28
    const BoneMask *mBoneMask; // +32
};

class FnDeltaLerpChan : public FnDeltaChan {
  public:
    FnDeltaLerpChan();
    virtual ~FnDeltaLerpChan();
    virtual void Eval(float prevTime, float currTime, float *evalBuffer);
    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);
    virtual bool EvalWeights(float currTime, float *weights);
    virtual bool EvalVel2D(float currTime, float *vel);
    virtual bool GetAnimatedBonesAux(BoneMask *, int &, int &, int &);

  protected:
    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask);
};

class FnDeltaQuatChan : public FnDeltaChan {
  public:
    FnDeltaQuatChan();
    virtual ~FnDeltaQuatChan();
    virtual void Eval(float prevTime, float currTime, float *evalBuffer);
    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);
    virtual bool GetAnimatedBonesAux(BoneMask *, int &, int &, int &);

  protected:
    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask);
};

// The variable index array starts at +14, immediately after the key count.
// The declaration describes this prefix, not a complete serialized allocation.
class KeyDeltaChan : public AnimMemoryMap {
  public:
    unsigned short GetNumKeys() const { return mNumKeys; }
    int GetNumDofs() const { return mData->GetNumDofs(); }
    unsigned short *GetDofIndices() { return reinterpret_cast<unsigned short *>(&mNumKeys + 1); }
    const unsigned short *GetDofIndices() const {
        return reinterpret_cast<const unsigned short *>(&mNumKeys + 1);
    }
    DeltaCompressedData *GetDeltaData() const { return mData; }
    unsigned short *GetKeyTimes() const { return mKeyTimes; }

  private:
    DeltaCompressedData *mData; // +4
    unsigned short *mKeyTimes;  // +8
    unsigned short mNumKeys;    // +12
};

class FnKeyDeltaChan : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    FnKeyDeltaChan();
    virtual ~FnKeyDeltaChan();
    virtual void SetAnimMemoryMap(AnimMemoryMap *anim);
    virtual bool GetLength(float &timeLength) const;

  protected:
    void EvalToPrevValues(int key);
    void EvalToPrevValues(int key, int numDofPerBone, int, const unsigned short *);
    int FindLowerKey(float currTime);
    int mPrevKey;              // +16
    float *mPrevValues;        // +20
    int mNumDofs;              // +24
    unsigned short *mDofMask;  // +28
    const BoneMask *mBoneMask; // +32
};

class KeyLerpChan : public KeyDeltaChan {};

class FnKeyLerpChan : public FnKeyDeltaChan {
  public:
    FnKeyLerpChan();
    virtual ~FnKeyLerpChan();
    virtual void Eval(float prevTime, float currTime, float *evalBuffer);
    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);
    virtual bool GetAnimatedBonesAux(BoneMask *, int &, int &, int &);

  protected:
    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask);
};

class FnKeyQuatChan : public FnKeyDeltaChan {
  public:
    FnKeyQuatChan();
    virtual ~FnKeyQuatChan();
    virtual void Eval(float prevTime, float currTime, float *evalBuffer);
    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);
    virtual bool GetAnimatedBonesAux(BoneMask *, int &, int &, int &);

  protected:
    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask);
};

class KeyQuatChan : public KeyDeltaChan {};

} // namespace EAGLAnim
#endif
