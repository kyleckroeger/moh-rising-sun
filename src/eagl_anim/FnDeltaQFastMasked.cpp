// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#pragma implementation "FnDeltaQFast.h"
#include "EAGLAnim/FnDeltaQFast.h"
namespace EAGLAnim {
void FnDeltaQFast::AddDeltaMask(DeltaQFastPhysical *floorPhys, DeltaQFast *deltaQ, int prevDeltaIdx,
                                int floorDeltaIdx, COORD4 *prevQs, const BoneMask *boneMask) {
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
    DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(floorPhys + deltaQ->mNumBones) +
                         prevDeltaIdx * deltaQ->mNumBones;
    COORD4 delta;
    for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
        for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                prevQs[ibone].x += delta.x;
                prevQs[ibone].y += delta.y;
                prevQs[ibone].z += delta.z;
                prevQs[ibone].w += delta.w;
            }
            d++;
        }
    }
}
void FnDeltaQFast::SubDeltaMask(DeltaQFastPhysical *floorPhys, DeltaQFast *deltaQ, int prevDeltaIdx,
                                int floorDeltaIdx, COORD4 *prevQs, const BoneMask *boneMask) {
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
    DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(floorPhys + deltaQ->mNumBones) +
                         (prevDeltaIdx * deltaQ->mNumBones - 1);
    COORD4 delta;
    for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
        for (int ibone = deltaQ->mNumBones - 1; ibone >= 0; ibone--) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                d->UnQuantize(mMinRangesf[ibone], delta);
                prevQs[ibone].x -= delta.x;
                prevQs[ibone].y -= delta.y;
                prevQs[ibone].z -= delta.z;
                prevQs[ibone].w -= delta.w;
            }
            d--;
        }
    }
}
void FnDeltaQFast::UpdateNextQsMask(DeltaQFast *deltaQ, int ceilKey, int floorBinIdx,
                                    int floorDeltaIdx, const BoneMask *boneMask) {
    if (ceilKey != mNextKey) {
        int ceilBinIdx = ceilKey >> deltaQ->GetBinLengthPower();
        DeltaQFastPhysical *phys =
            reinterpret_cast<DeltaQFastPhysical *>(mBins + ceilBinIdx * mBinSize);
        unsigned char *boneIdxs = deltaQ->mBoneIdxs;
        if (ceilBinIdx != floorBinIdx) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++)
                if (boneMask->GetBone(boneIdxs[ibone]))
                    phys[ibone].UnQuantize(mNextQs[ibone]);
        } else {
            DeltaQFastDelta *d = reinterpret_cast<DeltaQFastDelta *>(phys + deltaQ->mNumBones) +
                                 floorDeltaIdx * deltaQ->mNumBones;
            COORD4 delta;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    d->UnQuantize(mMinRangesf[ibone], delta);
                    mNextQs[ibone].x = delta.x + mPrevQs[ibone].x;
                    mNextQs[ibone].y = delta.y + mPrevQs[ibone].y;
                    mNextQs[ibone].z = delta.z + mPrevQs[ibone].z;
                    mNextQs[ibone].w = delta.w + mPrevQs[ibone].w;
                }
                d++;
            }
        }
        mNextKey = ceilKey;
    }
}
} // namespace EAGLAnim
