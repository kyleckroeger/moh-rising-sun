// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#pragma implementation "FnRunBlender.h"
#include "EAGLAnim/FnRunBlender.h"
namespace EAGLAnim {
FnRunBlender::FnRunBlender()
    : mAnims(0), mPhases(0), mVels(0), mWeight(0), mNumAnims(0), mIdx(-100), mFreq(1), mPrevTime(0),
      mOffset(0), mCycleIdx(-100), mInit(false), mSpeed(0) {
    mFnAnims[0] = 0;
    mFnAnims[1] = 0;
    mFnVelAnims[0] = 0;
    mFnVelAnims[1] = 0;
    mType = static_cast<AnimTypeId::Type>(8);
}
void FnRunBlender::Eval(float, float time, float *pose) { EvalSQT(time, pose, 0); }
float FnRunBlender::CycleTime(float t, float start, float end) const {
    float length = end - start;
    int periods;
    float delta;
    if (t < start) {
        delta = start - t;
        periods = FloatToInt(delta / length);
        return end - (delta - periods * length);
    } else if (t >= end) {
        delta = t - end;
        periods = FloatToInt(delta / length);
        return start + (delta - periods * length);
    }
    return t;
}
int FnRunBlender::ComputeCycleIdx(float t, float start, float end) const {
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
bool FnRunBlender::EvalPhase(float, PhaseValue &) { return false; }
} // namespace EAGLAnim
