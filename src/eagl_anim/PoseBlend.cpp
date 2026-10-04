// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnPoseBlender.h"
#include "EAGLAnim/RawPoseMath.h"
namespace EAGLAnim {
static inline void LinearBlendF3(float w, const float *a, const float *b, float *out) {
    out[0] = w * (b[0] - a[0]) + a[0];
    out[1] = w * (b[1] - a[1]) + a[1];
    out[2] = w * (b[2] - a[2]) + a[2];
}
void FnPoseBlender::Blend(int numBones, float w, const float *pose0, const float *pose1, float *result,
                          const BoneMask *mask) {
    int i;
    if (!mask) {
        int j = 0;
        for (i = 0; i < numBones; i++) {
            j += 4;
            FastQuatBlendF4(w, pose0 + j, pose1 + j, result + j);
            j += 4;
            LinearBlendF3(w, pose0 + j, pose1 + j, result + j);
            j += 4;
        }
    } else {
        int j = 0;
        for (i = 0; i < numBones; i++) {
            j += 4;
            if (mask->GetBone(i)) {
                FastQuatBlendF4(w, pose0 + j, pose1 + j, result + j);
                j += 4;
                LinearBlendF3(w, pose0 + j, pose1 + j, result + j);
                j += 4;
            } else
                j += 8;
        }
    }
}
} // namespace EAGLAnim
