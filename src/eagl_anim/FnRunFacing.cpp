// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnRunBlender.h"
#include "EAGLAnim/ScratchBuffer.h"
#include "EAGLAnim/RawPoseMath.h"
namespace EAGLAnim {
static inline void Rotate(COORD4 &out, const COORD4 &v, const COORD4 &q) {
    float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
    float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;
    float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
    float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
    out.x = v.x * (1.0f - (yy + zz)) + v.y * (xy - wz) + v.z * (xz + wy);
    out.y = v.x * (xy + wz) + v.y * (1.0f - (xx + zz)) + v.z * (yz - wx);
    out.z = v.x * (xz - wy) + v.y * (yz + wx) + v.z * (1.0f - (xx + yy));
    out.w = 1.0f;
}
bool FnRunBlender::BlendFacing(float t0, float t1, float *f) const {
    float *sqt = static_cast<float *>(ScratchBuffer::GetScratchBuffer(0).GetBuffer());
    if (!mFnAnims[0]->EvalSQT(t0, sqt, 0))
        return false;
    COORD4 q0 = *reinterpret_cast<COORD4 *>(sqt + 4);
    COORD4 q1, q;
    if (mWeight != 0) {
        mSkeleton->GetStillPose(sqt, 0);
        if (!mFnAnims[1]->EvalSQT(t1, sqt, 0))
            return false;
        q1 = *reinterpret_cast<COORD4 *>(sqt + 4);
        FastQuatBlendF4(mWeight, &q0.x, &q1.x, &q.x);
    } else
        q = q0;
    COORD4 v = {0, 1, 0, 1}, out;
    Rotate(out, v, q);
    f[0] = out.x;
    f[1] = out.z;
    return true;
}
} // namespace EAGLAnim
