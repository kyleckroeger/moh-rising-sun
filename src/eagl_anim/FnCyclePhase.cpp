// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCycle.h"
namespace EAGLAnim {
bool FnCycle::EvalPhase(float time, PhaseValue &phase) {
    return mpAnim->EvalPhase(GetInRangeTime(time), phase);
}
} // namespace EAGLAnim
