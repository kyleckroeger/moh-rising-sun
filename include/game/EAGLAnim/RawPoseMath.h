// AI-assisted reconstruction; attributed blend helper follows nfsmw CC0, see NOTICE.
#ifndef RAW_POSE_MATH_H
#define RAW_POSE_MATH_H
#include <math.h>
namespace EAGLAnim {
extern float qt0[7];
inline void EulerToQuaternion(const float *e, float *q) {
    float x = e[0] * 0.5f, y = e[1] * 0.5f, z = e[2] * 0.5f;
    float cx = cosf(x), cy = cosf(y), cz = cosf(z);
    float sx = sinf(x), sy = sinf(y), sz = sinf(z);
    float cc = cx * cz, cs = cx * sz, sc = sx * cz, ss = sx * sz;
    q[0] = cy * sc - sy * cs;
    q[1] = cy * ss + sy * cc;
    q[2] = cy * cs - sy * sc;
    q[3] = cy * cc + sy * ss;
}
inline void FastQuatBlendF4(float w, const float *d0, const float *d1, float *out) {
    if (d0[0] * d1[0] + d0[1] * d1[1] + d0[2] * d1[2] + d0[3] * d1[3] > 0.0f) {
        out[0] = w * (d1[0] - d0[0]) + d0[0];
        out[1] = w * (d1[1] - d0[1]) + d0[1];
        out[2] = w * (d1[2] - d0[2]) + d0[2];
        out[3] = w * (d1[3] - d0[3]) + d0[3];
    } else {
        out[0] = d0[0] - w * (d1[0] + d0[0]);
        out[1] = d0[1] - w * (d1[1] + d0[1]);
        out[2] = d0[2] - w * (d1[2] + d0[2]);
        out[3] = d0[3] - w * (d1[3] + d0[3]);
    }

    float s = 1.0f / sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2] + out[3] * out[3]);

    out[0] *= s;
    out[1] *= s;
    out[2] *= s;
    out[3] *= s;
}

} // namespace EAGLAnim
#endif
