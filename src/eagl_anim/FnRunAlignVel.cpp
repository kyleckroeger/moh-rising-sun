// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
namespace EAGLAnim {
static inline void Rotate(COORD4 &out, const COORD4 &v, const COORD4 &q) {
    float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
    float xx = q.x * x2, yy = q.y * y2, zz = q.z * z2;
    float xy = q.x * y2, xz = q.x * z2, yz = q.y * z2;
    float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;
    out.x = v.x * (1.0f - (yy + zz)) + v.y * (xy - wz) + v.z * (xz + wy);
    out.y = v.x * (xy + wz) + v.y * (1.0f - (xx + zz)) + v.z * (yz - wx);
    out.z = v.x * (xz - wy) + v.y * (yz + wx) + v.z * (1.0f - (xx + yy));
    out.w = 1.0f;
}
void FnRunBlender::AlignVel(float *vel) const {
    COORD4 v;
    v.x = vel[0];
    v.y = 0;
    v.z = vel[1];
    v.w = 1;
    COORD4 out;
    Rotate(out, v, mAlignQ);
    vel[0] = out.x;
    vel[1] = out.z;
}
} // namespace EAGLAnim
