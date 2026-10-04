// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
namespace EAGLAnim {
void QuatF4(float *&data, float *output) {
    output[0] = *data++;
    output[1] = *data++;
    output[2] = *data++;
    output[3] = *data++;
}
void TranF3(float *&data, float *output) {
    output[4] = *data++;
    output[5] = *data++;
    output[6] = *data++;
}
} // namespace EAGLAnim
