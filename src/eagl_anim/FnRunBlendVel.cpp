// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
#include <math.h>
namespace EAGLAnim {
bool FnRunBlender::BlendVel(float t0, float t1, float *vel) const {
    float v0[2], v1[2];
    if (!mFnVelAnims[0]->EvalVel2D(t0, v0))
        return false;
    if (mWeight != 0) {
        if (!mFnVelAnims[1]->EvalVel2D(t1, v1))
            return false;
        float w0 = 1.0f - mWeight;
        vel[0] = mWeight * v1[0] + w0 * v0[0];
        vel[1] = mWeight * v1[1] + w0 * v0[1];
        float len = sqrtf(vel[0] * vel[0] + vel[1] * vel[1]);
        if (len != 0) {
            float len1 = sqrtf(v1[0] * v1[0] + v1[1] * v1[1]);
            float len0 = sqrtf(v0[0] * v0[0] + v0[1] * v0[1]);
            float scale = (mWeight * len1 + w0 * len0) / len;
            vel[0] *= scale;
            vel[1] *= scale;
        }
    } else {
        vel[0] = v0[0];
        vel[1] = v0[1];
    }
    return true;
}
} // namespace EAGLAnim
