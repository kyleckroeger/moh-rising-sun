#pragma interface
// Scoped reconstructed interface of EAGLAnim::FnDeltaF1 (GR8E69 FnDeltaF1.cpp unit).
//
// Class name, method names and signatures: original mangled symbols.
// Layout (observed by the original constructor, destructor and evaluators):
//   +0x10 int mPrevKey, +0x14 void *mPrevVBlock, +0x18 float *mPrevValues,
//   +0x1c int mNextKey, +0x20 void *mNextVBlock, +0x24 float *mNextValues,
//   +0x28 const BoneMask *mBoneMask, +0x2c DeltaF1MinRange *mMinRangesf;
//   object size 48 (deleting destructor passes 48 to EAGLInternal::EAGLFree).
// Member names are research names from the attributed NFS Most Wanted reference.
// The in-class bodies below are emitted by the original unit after the three
// out-of-line functions; which of them were in-class in the original header is
// inferred from that emission order, not from a recovered header.
#ifndef MOH_EAGLANIM_FNDELTAF1_H
#define MOH_EAGLANIM_FNDELTAF1_H

#include "DeltaF1.h"

namespace EAGLAnim {

class FnDeltaF1 : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    FnDeltaF1() : mNextKey(-1), mNextValues(0), mPrevKey(-1), mPrevVBlock(0), mPrevValues(0), mNextVBlock(0) {
        mType = AnimTypeId::ANIM_DELTAF1;
    }

    virtual ~FnDeltaF1() {
        if (mPrevValues) {
            MemoryPoolManager::DeleteBlock(mPrevVBlock);
            MemoryPoolManager::DeleteBlock(mNextVBlock);
            MemoryPoolManager::DeleteBlock(mMinRangesf);
        }
    }

    virtual void SetAnimMemoryMap(AnimMemoryMap *anim) {
        mpAnim = anim;
        mPrevKey = -1;
        mNextKey = -1;
    }

    virtual bool GetLength(float &timeLength) const {
        DeltaF1 *deltaF = reinterpret_cast<DeltaF1 *>(mpAnim);

        timeLength = static_cast<float>(deltaF->GetNumFrames());
        return true;
    }

    virtual void Eval(float prevTime, float currTime, float *evalBuffer) { EvalSQT(currTime, evalBuffer, 0); }

    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);

    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask);

    virtual bool EvalWeights(float currTime, float *weights) {
        Eval(currTime, currTime, weights);
        return true;
    }

    virtual bool EvalVel2D(float currTime, float *vel) {
        Eval(currTime, currTime, vel);
        return true;
    }

    // Parameter names are descriptive: the three references receive the count,
    // minimum and maximum of the bones newly set in the mask.
    virtual bool GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
        DeltaF1 *deltaF = reinterpret_cast<DeltaF1 *>(mpAnim);

        // The original re-reads the index pointer every iteration (after SetBone).
        for (int i = deltaF->GetNumBones() - 1; i >= 0; i--) {
            unsigned short boneIdx = deltaF->GetDofIndices()[i] / 0xC;
            if (!boneMask->GetBone(boneIdx)) {
                if (numBones == 0) {
                    minBone = boneIdx;
                    maxBone = boneIdx;
                } else {
                    if (boneIdx < minBone) {
                        minBone = boneIdx;
                    }
                    if (boneIdx > maxBone) {
                        maxBone = boneIdx;
                    }
                }
                numBones++;
                boneMask->SetBone(boneIdx, true);
            }
        }
        return true;
    }

    void InitBuffersAsRequired();

  protected:
    int mPrevKey;
    void *mPrevVBlock;
    float *mPrevValues;
    int mNextKey;
    void *mNextVBlock;
    float *mNextValues;
    const BoneMask *mBoneMask;
    DeltaF1MinRange *mMinRangesf;
};

} // namespace EAGLAnim

#endif
