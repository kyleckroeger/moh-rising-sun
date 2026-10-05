// Target reconstruction with attributed CC0 nfsmw interface names.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_FN_TURN_BLENDER_H
#define MOH_FN_TURN_BLENDER_H
#pragma interface
#include "AnimCore.h"
#include "Skeleton.h"
#include "FnRunBlender.h"
#include "FnPoseBlender.h"
#include <math.h>
namespace EAGLAnim {
class FnTurnBlender : public FnAnim {
  public:
#include "AnimAllocation.h"
    FnTurnBlender();
    virtual ~FnTurnBlender();
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual void Eval(float, float, float *);
    void SetWeight(float);
    virtual bool EvalPhase(float, PhaseValue &);
    virtual bool EvalVel2D(float, float *);
    bool BlendVel(float, float, float *) const;

  private:
    void ComputeAlignQ(float *v1, float *v2, COORD4 &q) const {
        float dot = v1[0] * v2[0] + v1[1] * v2[1];
        float len1 = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
        float len2 = sqrtf(v2[0] * v2[0] + v2[1] * v2[1]);
        float cos2 = (dot / (len1 * len2) + 1.0f) * 0.5f;
        q.x = 0;
        q.y = sqrtf(1.0f - cos2);
        q.z = 0;
        q.w = sqrtf(cos2);
        if (v1[0] * v2[1] - v1[1] * v2[0] > 0)
            q.y = -q.y;
    }

    float CycleTime(float, float, float) const;
    int ComputeCycleIdx(float, float, float) const;
    void AlignCycleBeginEnd(int);
    void AlignRootQ(float *) const;
    void AlignVel(float *) const;
    bool BlendBeginFacing(float *) const;
    bool BlendEndFacing(float *) const;
    FnAnim **mAnims;
    FnAnim *mFnAnims[2];
    float mWeight;
    int mNumAnims, mIdx;
    Skeleton *mSkeleton;
    float mCycles[2], mOffsets[2], mFreq, mPrevTime, mOffset;
    int mCycleIdx;
    COORD4 mAlignQ;
    bool mInit;
};
} // namespace EAGLAnim
#endif
