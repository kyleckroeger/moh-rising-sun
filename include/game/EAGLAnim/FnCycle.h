// Attributed CC0 nfsmw interface; target-verified fields. See docs/Animation.md.
#ifndef MOH_FN_CYCLE_H
#define MOH_FN_CYCLE_H
#pragma interface
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
class FnCycle : public FnAnim {
  public:
    virtual ~FnCycle();
    static void operator delete(void *, unsigned int);
    virtual void Eval(float, float, float *);
    virtual bool EvalEvent(float, float, EventHandler **, void *);
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual bool EvalPhase(float, PhaseValue &);

  private:
    float GetInRangeTime(float t) const {
        int periods;
        if (t < mStartTime) {
            t -= mStartTime;
            periods = FloatToInt(t / mLength);
            return mEndTime - (t - periods * mLength);
        }
        if (t > mEndTime) {
            t -= mEndTime;
            periods = FloatToInt(t / mLength);
            return mStartTime + (t - periods * mLength);
        }
        return t;
    }
    float mStartTime, mEndTime, mLength;
    FnAnim *mpAnim;
};
} // namespace EAGLAnim
#endif
