// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnTurnBlender.h"
namespace EAGLAnim {
float FnTurnBlender::CycleTime(float t, float start, float end) const {
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
} // namespace EAGLAnim
