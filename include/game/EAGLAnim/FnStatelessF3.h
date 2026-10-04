// Scoped stateless animation control interface; evaluator bodies remain external.
// Layouts/signatures follow pinned target evidence; member names follow the CC0 nfsmw reference.
// See docs/Animation.md and src/eagl_anim/NOTICE.
#pragma interface
#ifndef MOH_FNSTATELESSF3_H
#define MOH_FNSTATELESSF3_H
#include "StatelessF3.h"
namespace EAGLAnim {
class FnStatelessF3 : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    virtual bool EvalSQT(float currTime, float *sqt, const BoneMask *boneMask);
    bool EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask, bool lerpReqd, int floorKey,
                     float scale);
    bool EvalSQTfast(float currTime, float *sqt, const BoneMask *boneMask, bool lerpReqd, int floorKey,
                     float scale);
    virtual unsigned short GetTargetCheckSum() const { return mpAnim->GetTargetCheckSum(); }
    FnStatelessF3() { mType = AnimTypeId::ANIM_STATELESSF3; }
    virtual ~FnStatelessF3() {}
    virtual void SetAnimMemoryMap(AnimMemoryMap *a) { mpAnim = a; }
    virtual bool GetLength(float &timeLength) const {
        StatelessF3 *a = reinterpret_cast<StatelessF3 *>(mpAnim);
        timeLength = static_cast<float>(a->GetNumFrames());
        return true;
    }
    virtual const AttributeBlock *GetAttributes() const {
        return reinterpret_cast<StatelessF3 *>(mpAnim)->mAttributeBlock;
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
        StatelessF3 *deltaF = reinterpret_cast<StatelessF3 *>(mpAnim);

        for (int i = deltaF->mNumBones - 1; i >= 0; i--) {
            unsigned short boneIdx = deltaF->mDofIdxs[i] / 0xC;
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

  protected:
    unsigned short mPrevKey;
    unsigned char mUseFPS, mFPS;
    const BoneMask *mBoneMask;
};
} // namespace EAGLAnim
#endif
