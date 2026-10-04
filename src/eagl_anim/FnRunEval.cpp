// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
#include "EAGLAnim/ScratchBuffer.h"
namespace EAGLAnim {
bool FnRunBlender::EvalSQT(float time, float *pose, const BoneMask *) {
    mPrevTime = time;
    if (!mFnAnims[0])
        SetWeight(0);
    time += mOffset;
    float t0 = mFreq * mCycles[0] * time + mAlignFrame[0];
    float t1 = mFreq * mCycles[1] * time + mAlignFrame[1];
    int cycle = ComputeCycleIdx(t0, 0, mPhases[mIdx]->mNumFrames - 1);
    t0 = CycleTime(t0, 0, mPhases[mIdx]->mNumFrames - 1);
    t1 = CycleTime(t1, 0, mPhases[mIdx + 1]->mNumFrames - 1);
    mSkeleton->GetStillPose(pose, 0);
    if (!mFnAnims[0]->EvalSQT(t0, pose, 0))
        return false;
    if (mWeight != 0) {
        float *temp = static_cast<float *>(ScratchBuffer::GetScratchBuffer(0).GetBuffer());
        mSkeleton->GetStillPose(temp, 0);
        if (!mFnAnims[1]->EvalSQT(t1, temp, 0))
            return false;
        FnPoseBlender::Blend(mSkeleton->GetNumBones(), mWeight, pose, temp, pose, 0);
    }
    AlignCycleBeginEnd(cycle);
    AlignRootQ(pose);
    return true;
}
} // namespace EAGLAnim
