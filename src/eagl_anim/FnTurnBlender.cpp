// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#pragma implementation "FnTurnBlender.h"
#include "EAGLAnim/FnTurnBlender.h"
#include "EAGLAnim/ScratchBuffer.h"
#include "EAGLAnim/RawPoseMath.h"
#include <stdio.h>
namespace EAGLAnim {
static inline void Rotate(COORD4 &out, const COORD4 &v, const COORD4 &q) {
    float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
    float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;
    float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
    float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
    out.x = v.x * (1.0f - (yy + zz)) + v.y * (xy - wz) + v.z * (xz + wy);
    out.y = v.x * (xy + wz) + v.y * (1.0f - (xx + zz)) + v.z * (yz - wx);
    out.z = v.x * (xz - wy) + v.y * (yz + wx) + v.z * (1.0f - (xx + yy));
    out.w = 1.0f;
}
static inline void MultiplyQ(const float *a, const float *b, float *r) {
    r[0] = a[0] * b[3] - a[1] * b[2] + a[2] * b[1] + a[3] * b[0];
    r[1] = a[0] * b[2] + a[1] * b[3] - a[2] * b[0] + a[3] * b[1];
    r[2] = -a[0] * b[1] + a[1] * b[0] + a[2] * b[3] + a[3] * b[2];
    r[3] = -a[0] * b[0] - a[1] * b[1] - a[2] * b[2] + a[3] * b[3];
}

bool FnTurnBlender::EvalSQT(float time, float *pose, const BoneMask *) {
    mPrevTime = time;
    if (!mFnAnims[0])
        SetWeight(0);
    time += mOffset;
    int cycle = ComputeCycleIdx(time, 0, 2.0f / mFreq);
    float t0 = mFreq * mCycles[0] * time;
    float t1 = mFreq * mCycles[1] * time;
    t0 = CycleTime(t0, 0, mCycles[0] * 2) - mOffsets[0];
    t1 = CycleTime(t1, 0, mCycles[1] * 2) - mOffsets[1];
    mSkeleton->GetStillPose(pose, 0);
    if (!mFnAnims[0]->EvalSQT(t0, pose, 0))
        return false;
    if (mWeight != 0) {
        float *temp = static_cast<float *>(ScratchBuffer::GetScratchBuffer(1).GetBuffer());
        mSkeleton->GetStillPose(temp, 0);
        if (!mFnAnims[1]->EvalSQT(t1, temp, 0))
            return false;
        FnPoseBlender::Blend(mSkeleton->GetNumBones(), mWeight, pose, temp, pose, 0);
    }
    AlignCycleBeginEnd(cycle);
    AlignRootQ(pose);
    return true;
}
void FnTurnBlender::SetWeight(float w) {
    float freq;
    int idx = FloatToInt(w);
    if (idx < 0)
        idx = 0;
    if (idx >= mNumAnims - 1)
        idx = mNumAnims - 2;
    mWeight = w - idx;
    if (idx == mIdx) {
        freq = mFreq;
        mFreq = mWeight / mCycles[1] + (1.0f - mWeight) / mCycles[0];
        mOffset = freq / mFreq * (mPrevTime + mOffset) - mPrevTime;
    } else {
        if (idx == mIdx + 1) {
            mFnAnims[0] = mFnAnims[1];
            mFnAnims[1] = mAnims[idx + 1];
        } else if (idx == mIdx - 1) {
            mFnAnims[1] = mFnAnims[0];
            mFnAnims[0] = mAnims[idx];
        } else {
            mFnAnims[0] = mAnims[idx];
            mFnAnims[1] = mAnims[idx + 1];
        }
        mIdx = idx;
        FnRunBlender *run = static_cast<FnRunBlender *>(mAnims[mIdx]);
        mCycles[0] = 1.0f / run->GetFrequency();
        mOffsets[0] = run->GetOffset();
        run = static_cast<FnRunBlender *>(mAnims[mIdx + 1]);
        mCycles[1] = 1.0f / run->GetFrequency();
        mOffsets[1] = run->GetOffset();
        freq = mFreq;
        mFreq = mWeight / mCycles[1] + (1.0f - mWeight) / mCycles[0];
        mOffset = freq / mFreq * (mPrevTime + mOffset) - mPrevTime;
    }
}
bool FnTurnBlender::EvalVel2D(float time, float *vel) {
    mPrevTime = time;
    if (!mFnAnims[0])
        SetWeight(0);
    time += mOffset;
    int cycle = ComputeCycleIdx(time, 0, 2.0f / mFreq);
    printf("currTime: %g  offset: %g   cycle: %g\n", time, mOffset, 1.0f / mFreq);
    printf("offset0: %g  offset1: %g\n", mOffsets[0], mOffsets[1]);
    printf("cycle0: %g  cycle1: %g\n", mCycles[0], mCycles[1]);
    float t0 = mFreq * mCycles[0] * time;
    float t1 = mFreq * mCycles[1] * time;
    printf("before offset t0: %g  t1: %g\n", t0, t1);
    t0 = CycleTime(t0, 0, 2 * mCycles[0]) - mOffsets[0];
    t1 = CycleTime(t1, 0, 2 * mCycles[1]) - mOffsets[1];
    if (!BlendVel(t0, t1, vel))
        return false;
    AlignCycleBeginEnd(cycle);
    AlignVel(vel);
    return true;
}

bool FnTurnBlender::BlendVel(float t0, float t1, float *vel) const {
    float v0[2], v1[2];
    if (!mFnAnims[0]->EvalVel2D(t0, v0))
        return false;
    if (mWeight != 0) {
        if (!mFnAnims[1]->EvalVel2D(t1, v1))
            return false;
        float w0 = 1.0f - mWeight;
        vel[0] = mWeight * v1[0] + w0 * v0[0];
        vel[1] = mWeight * v1[1] + w0 * v0[1];
        float len = sqrtf(vel[0] * vel[0] + vel[1] * vel[1]);
        if (len != 0) {
            float len1 = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
            float len0 = sqrtf(v0[0] * v0[0] + v0[1] * v0[1]);
            float scale = (mWeight * len1 + w0 * len0) / len;
            vel[0] *= scale;
            vel[1] *= scale;
        }
    } else {
        vel[0] = v0[0];
        vel[1] = v0[1];
    }
    return true;
}

void FnTurnBlender::AlignCycleBeginEnd(int cycle) {
    if (!mInit) {
        mCycleIdx = -1;
        mAlignQ.x = 0;
        mAlignQ.y = 0;
        mAlignQ.z = 0;
        mAlignQ.w = 1;
        mInit = true;
    } else if (mCycleIdx != cycle) {
        COORD4 q, result;
        float begin[2], end[2];
        BlendBeginFacing(begin);
        BlendEndFacing(end);
        ComputeAlignQ(begin, end, q);
        if (mCycleIdx - 1 == cycle)
            q.y = -q.y;
        MultiplyQ(&mAlignQ.x, &q.x, &result.x);
        mAlignQ = result;
        mCycleIdx = cycle;
        static int i = 0;
        printf("turn align[%d] Q: %g %g %g %g\n\n", i++, mAlignQ.x, mAlignQ.y, mAlignQ.z,
               mAlignQ.w);
    }
}

void FnTurnBlender::AlignVel(float *vel) const {
    COORD4 v;
    v.x = vel[0];
    v.y = 0;
    v.z = vel[1];
    v.w = 1;
    COORD4 out;
    Rotate(out, v, mAlignQ);
    vel[0] = out.x;
    vel[1] = out.z;
}
bool FnTurnBlender::BlendBeginFacing(float *f) const {
    COORD4 q0, q1, q;
    static_cast<FnRunBlender *>(mFnAnims[0])->ComputeBeginRootQ(q0);
    static_cast<FnRunBlender *>(mFnAnims[1])->ComputeBeginRootQ(q1);
    FastQuatBlendF4(mWeight, &q0.x, &q1.x, &q.x);
    COORD4 v = {0, 1, 0, 1}, out;
    Rotate(out, v, q);
    f[0] = out.x;
    f[1] = out.z;
    printf("Facing: %g %g\n", f[0], f[1]);
    return true;
}
bool FnTurnBlender::BlendEndFacing(float *f) const {
    COORD4 q0, q1, q;
    static_cast<FnRunBlender *>(mFnAnims[0])->ComputeEndRootQ(q0);
    static_cast<FnRunBlender *>(mFnAnims[1])->ComputeEndRootQ(q1);
    FastQuatBlendF4(mWeight, &q0.x, &q1.x, &q.x);
    COORD4 v = {0, 1, 0, 1}, out;
    Rotate(out, v, q);
    f[0] = out.x;
    f[1] = out.z;
    printf("Facing: %g %g\n", f[0], f[1]);
    return true;
}

FnTurnBlender::FnTurnBlender()
    : mAnims(0), mWeight(0), mNumAnims(0), mIdx(-100), mFreq(1), mPrevTime(0), mOffset(0),
      mCycleIdx(-100), mInit(false) {
    mFnAnims[0] = 0;
    mFnAnims[1] = 0;
    mType = static_cast<AnimTypeId::Type>(9);
}
FnTurnBlender::~FnTurnBlender() {
    if (mNumAnims)
        ScratchBuffer::GetScratchBuffer(1).FreeBuffer();
    if (mAnims)
        MemoryPoolManager::DeleteBlock(mAnims);
}
void FnTurnBlender::Eval(float, float time, float *pose) { EvalSQT(time, pose, 0); }
bool FnTurnBlender::EvalPhase(float, PhaseValue &) { return false; }
int FnTurnBlender::ComputeCycleIdx(float t, float start, float end) const {
    float length = end - start;
    if (t < start) {
        t = start - t;
        return FloatToInt(t / length);
    }
    if (t >= end) {
        t -= end;
        return FloatToInt(t / length) + 1;
    }
    return 0;
}
void FnTurnBlender::AlignRootQ(float *sqt) const {
    COORD4 out;
    MultiplyQ(sqt + 4, &mAlignQ.x, &out.x);
    *reinterpret_cast<COORD4 *>(sqt + 4) = out;
}

} // namespace EAGLAnim
