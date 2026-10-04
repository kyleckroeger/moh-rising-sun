// Scoped stateless animation control interface; evaluator bodies remain external.
// Layouts/signatures follow pinned target evidence; member names follow the CC0 nfsmw reference.
// See docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_STATELESSF3_H
#define MOH_STATELESSF3_H
#include "AnimCore.h"
#include "EAGLTransform.h"
namespace EAGLAnim {
struct StatelessF3 : public AnimMemoryMap {
    AttributeBlock *mAttributeBlock;
    unsigned short *mTimes;
    unsigned short *mDofIdxs;
    unsigned short mNumKeys;
    unsigned char mNumBones, mNumConstBones;
    unsigned char unknown_14[4];
    unsigned short GetNumFrames() const {
        if (!mTimes)
            return mNumKeys;
        return mTimes[mNumKeys - 2] + 1;
    }
};
} // namespace EAGLAnim
#endif
