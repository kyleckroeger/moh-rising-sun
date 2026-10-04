// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnRawEventChannel.h"
namespace EAGLAnim {
FnRawEventChannel::~FnRawEventChannel() {}
void FnRawEventChannel::SetAnimMemoryMap(AnimMemoryMap *anim) {
    mpAnim = anim;
    mCurrentIdx = 0;
    mCurrentTime = 0.0f;
}
bool FnRawEventChannel::EvalEvent(float previousTime, float currentTime, EventHandler **handlers,
                                  void *extra) {
    reinterpret_cast<RawEventChannel *>(mpAnim)->Eval(previousTime, currentTime, mCurrentIdx, mCurrentTime,
                                                      handlers, extra);
    return true;
}
void FnRawEventChannel::Eval(float previousTime, float currentTime, float *handlers) {
    EvalEvent(previousTime, currentTime, reinterpret_cast<EventHandler **>(handlers), 0);
}
} // namespace EAGLAnim
