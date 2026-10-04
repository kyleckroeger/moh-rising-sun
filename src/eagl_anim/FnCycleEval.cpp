// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCycle.h"
namespace EAGLAnim {
void FnCycle::Eval(float previousTime, float currentTime, float *dofs) {
    mpAnim->Eval(GetInRangeTime(previousTime), GetInRangeTime(currentTime), dofs);
}
} // namespace EAGLAnim
