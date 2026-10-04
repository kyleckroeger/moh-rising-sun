// Scoped stateless animation control interface; evaluator bodies remain external.
// Layouts/signatures follow pinned target evidence; member names follow the CC0 nfsmw reference.
// See docs/Animation.md and src/eagl_anim/NOTICE.
#pragma interface
#ifndef MOH_FNSTATELESSQ_H
#define MOH_FNSTATELESSQ_H
#include "StatelessQ.h"
#include "FnStatelessF3.h"
namespace EAGLAnim {
class FnStatelessQ : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);
    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask, bool lerpReqd, int floorKey,
                     float scale);
    virtual unsigned short GetTargetCheckSum() const { return mpAnim->GetTargetCheckSum(); }
    FnStatelessQ() {
        mType = AnimTypeId::ANIM_STATELESSQ;
        mPrevKey = 0;
    }
    virtual ~FnStatelessQ() {}
    virtual void SetAnimMemoryMap(AnimMemoryMap *a) { mpAnim = a; }
    virtual bool GetLength(float &timeLength) const {
        StatelessQ *a = reinterpret_cast<StatelessQ *>(mpAnim);
        timeLength = static_cast<float>(a->GetNumFrames());
        return true;
    }
    virtual const AttributeBlock *GetAttributes() const {
        return reinterpret_cast<StatelessQ *>(mpAnim)->mAttributeBlock;
    }
    virtual void Eval(float, float currTime, float *sqt) { EvalSQT(currTime, sqt, 0); }
    virtual void UseFPS(bool u) {
        mUseFPS = u;
        if (mFPS != 0 || !mUseFPS)
            return;
        GetAttribute(AttributeId(AttributeId::ID_FPS), mFPS);
    }
    virtual bool GetAnimatedBones(BoneMask *mask, int &count, int &minBone, int &maxBone) {
        mask->SetAll(false);
        count = 0;
        return GetAnimatedBonesAux(mask, count, minBone, maxBone);
    }
    virtual bool GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
        StatelessQ *deltaF = reinterpret_cast<StatelessQ *>(mpAnim);

        for (int i = deltaF->mNumBones - 1; i >= 0; i--) {
            unsigned char boneIdx = deltaF->mBoneIdxs[i];
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
        if (deltaF->mF3Ptr)
            return deltaF->mF3Ptr->GetAnimatedBonesAux(boneMask, numBones, minBone, maxBone);
        return true;
    }

  protected:
    unsigned short mPrevKey;
    unsigned char mUseFPS, mFPS;
    const BoneMask *mBoneMask;
};
} // namespace EAGLAnim
#endif
