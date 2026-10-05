// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnDeltaQFast.h"
namespace EAGLAnim {
void FnDeltaQFast::SetAnimMemoryMap(AnimMemoryMap *anim) {
    mpAnim = anim;
    DeltaQFast *deltaQ = reinterpret_cast<DeltaQFast *>(anim);
    DeltaQFastMinRange *ranges = deltaQ->GetMinRange();
    mBins = reinterpret_cast<unsigned char *>(ranges + deltaQ->mNumBones);
    mConstBoneIdxs = deltaQ->GetConstBoneIdx();
    mConstPhysical = deltaQ->GetConstPhysical();
    mBinSize = deltaQ->GetBinSize();
    InitBuffers(deltaQ);
}
} // namespace EAGLAnim
