// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_FN_CSIS_EVENT_H
#define MOH_FN_CSIS_EVENT_H
#pragma interface
#include "CsisEventChannel.h"
namespace EAGLAnim {
class FnCsisEventChannel : public FnAnimMemoryMap {
  public:
    virtual ~FnCsisEventChannel();
    static void operator delete(void *, unsigned int);
    virtual void SetAnimMemoryMap(AnimMemoryMap *);
    virtual bool EvalEvent(float, float, EventHandler **, void *);
    virtual void Eval(float, float, float *);

  private:
    int mCurrentIdx;
    float mCurrentTime;
};
} // namespace EAGLAnim
#endif
