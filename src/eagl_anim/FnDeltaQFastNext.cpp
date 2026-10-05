// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnDeltaQFast.h"
namespace EAGLAnim {
void FnDeltaQFast::UpdateNextQs(DeltaQFast *deltaQ, int ceilKey, int floorBinIdx,
                                int floorDeltaIdx) {
    if (ceilKey != mNextKey) {
        int ceilBinIdx = ceilKey >> deltaQ->GetBinLengthPower();
        DeltaQFastPhysical *phys =
            reinterpret_cast<DeltaQFastPhysical *>(mBins + ceilBinIdx * mBinSize);
        if (ceilBinIdx != floorBinIdx) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++)
                phys[ibone].UnQuantize(mNextQs[ibone]);
        } else {
            DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(phys + deltaQ->mNumBones) +
                                 floorDeltaIdx * deltaQ->mNumBones;
            COORD4 delta;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                mNextQs[ibone].x = delta.x + mPrevQs[ibone].x;
                mNextQs[ibone].y = delta.y + mPrevQs[ibone].y;
                mNextQs[ibone].z = delta.z + mPrevQs[ibone].z;
                mNextQs[ibone].w = delta.w + mPrevQs[ibone].w;
                d++;
            }
        }
        mNextKey = ceilKey;
    }
}
} // namespace EAGLAnim
