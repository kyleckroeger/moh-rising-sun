// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCsisEventChannel.h"
namespace EAGLAnim {
FnCsisEventChannel::~FnCsisEventChannel() {}
void FnCsisEventChannel::SetAnimMemoryMap(AnimMemoryMap *anim) {
    mpAnim = anim;
    mCurrentIdx = 0;
    mCurrentTime = 0.0f;
}
bool FnCsisEventChannel::EvalEvent(float previousTime, float currentTime, EventHandler **handlers,
                                   void *extra) {
    reinterpret_cast<CsisEventChannel *>(mpAnim)->Eval(previousTime, currentTime, mCurrentIdx, mCurrentTime,
                                                       handlers, extra);
    return true;
}
void FnCsisEventChannel::Eval(float previousTime, float currentTime, float *handlers) {
    EvalEvent(previousTime, currentTime, reinterpret_cast<EventHandler **>(handlers), 0);
}
} // namespace EAGLAnim
