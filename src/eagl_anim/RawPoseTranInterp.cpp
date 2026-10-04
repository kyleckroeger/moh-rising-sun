// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
#include "EAGLAnim/RawPoseMath.h"
namespace EAGLAnim {
inline void TranF3(float *&data, float *output) {
    output[4] = *data++;
    output[5] = *data++;
    output[6] = *data++;
}
void TranF3Interp(float w, float *&data0, float *&data1, float *output) {
    TranF3(data0, qt0);
    TranF3(data1, output);
    float *d0 = qt0 + 4, *d1 = output + 4;
    d1[0] = w * (d1[0] - d0[0]) + d0[0];
    d1[1] = w * (d1[1] - d0[1]) + d0[1];
    d1[2] = w * (d1[2] - d0[2]) + d0[2];
}
} // namespace EAGLAnim
