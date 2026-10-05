// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnRunBlender.h"
namespace EAGLAnim {
void FnRunBlender::ComputeEndRootQ(COORD4 &q) const {
    ComputeRootQ(mPhases[mIdx]->mNumFrames - 1, mPhases[mIdx + 1]->mNumFrames - 1, q);
}
} // namespace EAGLAnim
