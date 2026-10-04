#pragma interface
// Scoped reconstructed interface of EAGLAnim::FnDeltaF3 (GR8E69 FnDeltaF3.cpp unit).
//
// Class name, method names and signatures: original mangled symbols.
// Layout observed by the original constructor/destructor/evaluators
// (0x801f350c, 0x801f3570, 0x801f330c): identical to FnDeltaF1
// except the value buffers hold 16-byte COORD4 elements and the ranges are
// 48-byte DeltaF3MinRange records. mType = 20. Object size 48.
// Member names are research names from the attributed NFS Most Wanted reference.
// In-class bodies below follow the original emission order (three out-of-line
// functions first, then these eight in declaration order).
#ifndef MOH_EAGLANIM_FNDELTAF3_H
#define MOH_EAGLANIM_FNDELTAF3_H

#include "DeltaF3.h"

namespace EAGLAnim {

class FnDeltaF3 : public FnAnimMemoryMap {
  public:
    FnDeltaF3() : mNextKey(-1), mNextValues(0), mPrevKey(-1), mPrevVBlock(0), mPrevValues(0), mNextVBlock(0) {
        mType = AnimTypeId::ANIM_DELTAF3;
    }

    virtual ~FnDeltaF3() {
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
        DeltaF3 *deltaF = reinterpret_cast<DeltaF3 *>(mpAnim);

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

    // Parameter names are descriptive (count, minimum and maximum of newly set bones).
    virtual bool GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
        DeltaF3 *deltaF = reinterpret_cast<DeltaF3 *>(mpAnim);

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
    COORD4 *mPrevValues;
    int mNextKey;
    void *mNextVBlock;
    COORD4 *mNextValues;
    const BoneMask *mBoneMask;
    DeltaF3MinRange *mMinRangesf;
};

} // namespace EAGLAnim

#endif
