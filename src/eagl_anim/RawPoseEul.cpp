// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
#include "EAGLAnim/RawPoseMath.h"
namespace EAGLAnim {
inline float DegreesToRadians(float degrees) { return degrees * 0.017453292f; }
void EulF3(float *&data, float *output) {
    float e[3];
    e[0] = DegreesToRadians(*data++);
    e[1] = DegreesToRadians(*data++);
    e[2] = DegreesToRadians(*data++);
    EulerToQuaternion(e, output);
}
} // namespace EAGLAnim
