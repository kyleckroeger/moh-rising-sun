// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCycle.h"
namespace EAGLAnim {
bool FnCycle::EvalSQT(float time, float *sqt, const BoneMask *mask) {
    return mpAnim->EvalSQT(GetInRangeTime(time), sqt, mask);
}
} // namespace EAGLAnim
