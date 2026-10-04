// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
namespace EAGLAnim {
static inline void MultiplyQ(const float *a, const float *b, float *r) {
    r[0] = a[0] * b[3] - a[1] * b[2] + a[2] * b[1] + a[3] * b[0];
    r[1] = a[0] * b[2] + a[1] * b[3] - a[2] * b[0] + a[3] * b[1];
    r[2] = -a[0] * b[1] + a[1] * b[0] + a[2] * b[3] + a[3] * b[2];
    r[3] = -a[0] * b[0] - a[1] * b[1] - a[2] * b[2] + a[3] * b[3];
}
void FnRunBlender::AlignRootQ(float *sqt) const {
    COORD4 out;
    MultiplyQ(sqt + 4, &mAlignQ.x, &out.x);
    *reinterpret_cast<COORD4 *>(sqt + 4) = out;
}
} // namespace EAGLAnim
