// Scoped target interface and attributed reference names; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_FN_RUN_BLENDER_H
#define MOH_FN_RUN_BLENDER_H
#pragma interface
#include "AnimCore.h"
#include "Skeleton.h"
#include "PhaseChan.h"
#include "FnPoseBlender.h"
namespace EAGLAnim {
class FnRunBlender : public FnAnim {
  public:
#include "AnimAllocation.h"
    FnRunBlender();
    virtual ~FnRunBlender();
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual void Eval(float, float, float *);
    void SetWeight(float);
    virtual bool EvalPhase(float, PhaseValue &);
    virtual bool EvalVel2D(float, float *);
    virtual bool FindMatchTime(const MatchPhaseInput &, float &) const;
    float GetFrequency() const;
    void ComputeBeginRootQ(COORD4 &) const;
    void ComputeEndRootQ(COORD4 &) const;

  private:
    void ComputeRootQ(float, float, COORD4 &) const;
    float CycleTime(float, float, float) const;
    int ComputeCycleIdx(float, float, float) const;
    void AlignCycleBeginEnd(int);
    void AlignRootQ(float *) const;
    void AlignVel(float *) const;
    bool BlendVel(float, float, float *) const;
    bool BlendFacing(float, float, float *) const;
    const AnimMemoryMap **mAnims;
    const PhaseChan **mPhases;
    const AnimMemoryMap **mVels;
    FnAnim *mFnAnims[2];
    FnAnim *mFnVelAnims[2];
    float mWeight;
    int mNumAnims, mIdx;
    Skeleton *mSkeleton;
    float mAlignFrame[2], mCycles[2], mFreq, mPrevTime, mOffset;
    int mCycleIdx;
    COORD4 mAlignQ;
    bool mInit;
    // Reference calls +6c a root quaternion; no completed method proves its contents.
    unsigned char unknown_6c[16];
    // Owned allocation at +7c; destructor proves ownership but not element type.
    void *mSpeed;
};
} // namespace EAGLAnim
#endif
