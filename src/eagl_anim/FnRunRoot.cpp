// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
#include "EAGLAnim/ScratchBuffer.h"
#include <math.h>
namespace EAGLAnim {
// Adapted from CC0 nfsmw AnimUtil.h, commit 1f2cdd7996791c81a580b3f7b36b44d4f9f6719c.
static inline void FastQuatBlendF4(float w, const float *d0, const float *d1, float *out) {
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

void FnRunBlender::ComputeRootQ(float t0, float t1, COORD4 &q) const {
    float *sqt = static_cast<float *>(ScratchBuffer::GetScratchBuffer(0).GetBuffer());
    if (mFnAnims[0]->EvalSQT(t0, sqt, 0)) {
        COORD4 q0 = *reinterpret_cast<COORD4 *>(sqt + 4);
        if (mWeight != 0) {
            mSkeleton->GetStillPose(sqt, 0);
            if (mFnAnims[1]->EvalSQT(t1, sqt, 0)) {
                COORD4 q1 = *reinterpret_cast<COORD4 *>(sqt + 4);
                FastQuatBlendF4(mWeight, &q0.x, &q1.x, &q.x);
            }
        } else
            q = q0;
    }
}
} // namespace EAGLAnim
