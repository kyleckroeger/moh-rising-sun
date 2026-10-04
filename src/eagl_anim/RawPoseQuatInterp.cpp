// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
#include "EAGLAnim/RawPoseMath.h"
namespace EAGLAnim {
inline void QuatF4(float *&data, float *output) {
    output[0] = *data++;
    output[1] = *data++;
    output[2] = *data++;
    output[3] = *data++;
}
void QuatF4Interp(float w, float *&data0, float *&data1, float *output) {
    QuatF4(data0, qt0);
    QuatF4(data1, output);
    FastQuatBlendF4(w, qt0, output, output);
}
} // namespace EAGLAnim
