// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnRunBlender.h"
namespace EAGLAnim {
float FnRunBlender::GetFrequency() const { return mFreq; }
void FnRunBlender::ComputeBeginRootQ(COORD4 &q) const { ComputeRootQ(0, 0, q); }
} // namespace EAGLAnim
