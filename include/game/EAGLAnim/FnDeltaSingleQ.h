// Reconstructed single-axis quaternion format; target layouts and attributed reference names
// are documented in docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_FNDELTASINGLEQ_H
#define MOH_FNDELTASINGLEQ_H
#pragma interface
#include "DeltaSingleQ.h"
namespace EAGLAnim {
class FnDeltaSingleQ : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    FnDeltaSingleQ()
        : mMinRanges(0), mBins(0), mBinSize(-1), mPrevKey(-1), mPrevQBlock(0), mPrevQs(0), mPreMultQs(0),
          mPostMultQs(0) {
        mType = static_cast<AnimTypeId::Type>(19);
    }
    virtual ~FnDeltaSingleQ() {
        if (mPrevQBlock) {
            MemoryPoolManager::DeleteBlock(mPrevQBlock);
            MemoryPoolManager::DeleteBlock(mPreMultQs);
            MemoryPoolManager::DeleteBlock(mPostMultQs);
        }
    }
    virtual void SetAnimMemoryMap(AnimMemoryMap *anim) { mpAnim = anim; }

    void InitBuffersAsRequired() {
        if (!mPrevQs) {
            DeltaSingleQ *deltaQ = reinterpret_cast<DeltaSingleQ *>(mpAnim);
            DeltaSingleQMinRange *ranges;
            deltaQ->GetArrays(ranges, mBins);
            mBinSize = deltaQ->GetBinSize();
            void *block = MemoryPoolManager::NewBlock(deltaQ->mNumBones * sizeof(COORD4));
            mMinRanges = ranges;
            mPrevQBlock = block;
            mPrevQs = reinterpret_cast<COORD4 *>(block);
            mPreMultQs =
                reinterpret_cast<COORD4 *>(MemoryPoolManager::NewBlock(deltaQ->mNumBones * sizeof(COORD4)));
            mPostMultQs =
                reinterpret_cast<COORD4 *>(MemoryPoolManager::NewBlock(deltaQ->mNumBones * sizeof(COORD4)));
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                COORD3 e;
                DeltaSingleQMinRangef minRange;
                mMinRanges[ibone].UnQuantize(minRange);
                const DeltaSingleQMinRangef &range = minRange;
                if (range.mIndex == 0) {
                    mPreMultQs[ibone].x = 0.0f;
                    mPreMultQs[ibone].y = 0.0f;
                    mPreMultQs[ibone].z = 0.0f;
                    mPreMultQs[ibone].w = 1.0f;
                    e.x = 0.0f;
                    e.y = range.mConst0;
                    e.z = range.mConst1;
                    SingleQFromEuler(e, mPostMultQs[ibone]);
                } else if (range.mIndex == 1) {
                    e.x = range.mConst0;
                    e.y = e.z = 0.0f;
                    SingleQFromEuler(e, mPreMultQs[ibone]);
                    e.x = e.y = 0.0f;
                    e.z = range.mConst1;
                    SingleQFromEuler(e, mPostMultQs[ibone]);
                } else {
                    e.x = range.mConst0;
                    e.y = range.mConst1;
                    e.z = 0.0f;
                    SingleQFromEuler(e, mPreMultQs[ibone]);
                    mPostMultQs[ibone].x = 0.0f;
                    mPostMultQs[ibone].y = 0.0f;
                    mPostMultQs[ibone].z = 0.0f;
                    mPostMultQs[ibone].w = 1.0f;
                }
            }
        }
    }
    virtual bool GetLength(float &length) const {
        DeltaSingleQ *deltaQ = reinterpret_cast<DeltaSingleQ *>(mpAnim);
        length = static_cast<float>(deltaQ->GetNumFrames());
        return true;
    }
    virtual void Eval(float prevTime, float currTime, float *evalBuffer) {
        EvalSQTMasked(currTime, 0, evalBuffer);
    }
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual bool GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
        DeltaSingleQ *deltaQ = reinterpret_cast<DeltaSingleQ *>(mpAnim);
        for (int i = deltaQ->mNumBones - 1; i >= 0; i--) {
            unsigned char boneIdx = deltaQ->mBoneIdxs[i];
            if (!boneMask->GetBone(boneIdx)) {
                if (numBones == 0) {
                    minBone = boneIdx;
                    maxBone = boneIdx;
                } else {
                    if (boneIdx < minBone)
                        minBone = boneIdx;
                    if (boneIdx > maxBone)
                        maxBone = boneIdx;
                }
                numBones++;
                boneMask->SetBone(boneIdx, true);
            }
        }
        return true;
    }

  protected:
    virtual bool EvalSQTMasked(float, const BoneMask *, float *);
    DeltaSingleQMinRange *mMinRanges;
    unsigned char *mBins;
    int mBinSize, mPrevKey;
    void *mPrevQBlock;
    COORD4 *mPrevQs;
    COORD4 *mPreMultQs;
    COORD4 *mPostMultQs;
};
} // namespace EAGLAnim
#endif
