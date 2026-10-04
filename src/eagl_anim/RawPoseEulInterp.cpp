// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
#include "EAGLAnim/RawPoseMath.h"
namespace EAGLAnim {
inline float DegreesToRadians(float degrees) { return degrees * 0.017453292f; }
inline void EulF3(float *&data, float *output) {
    float e[3];
    e[0] = DegreesToRadians(*data++);
    e[1] = DegreesToRadians(*data++);
    e[2] = DegreesToRadians(*data++);
    EulerToQuaternion(e, output);
}
void EulF3Interp(float w, float *&data0, float *&data1, float *output) {
    EulF3(data0, qt0);
    EulF3(data1, output);
    FastQuatBlendF4(w, qt0, output, output);
}
} // namespace EAGLAnim
