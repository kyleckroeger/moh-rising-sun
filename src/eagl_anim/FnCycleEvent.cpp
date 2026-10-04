// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCycle.h"
namespace EAGLAnim {
bool FnCycle::EvalEvent(float previousTime, float currentTime, EventHandler **eventHandlers,
                        void *extraData) {
    return mpAnim->EvalEvent(GetInRangeTime(previousTime), GetInRangeTime(currentTime), eventHandlers,
                             extraData);
}
} // namespace EAGLAnim
