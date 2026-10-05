// Scoped target interface; reference names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_DELTAQ_H
#define MOH_DELTAQ_H
#include "AnimCore.h"
#include "EAGLTransform.h"
namespace EAGLAnim {
struct DeltaQMinRange;
struct DeltaQPhysical;
struct DeltaQ : AnimMemoryMap {
    unsigned short mNumKeys;
    unsigned char mNumBones, mNumConstBones;
    unsigned short *mTimes;
    unsigned char *mBoneIdxs;
    unsigned char mBinLengthPower, mPadding;
    int GetNumFrames() const {
        if (!mTimes)
            return mNumKeys;
        else
            return mTimes[mNumKeys - 2] + 1;
    }
};
} // namespace EAGLAnim
#endif
