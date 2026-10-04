// Scoped stateless animation control interface; evaluator bodies remain external.
// Layouts/signatures follow pinned target evidence; member names follow the CC0 nfsmw reference.
// See docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_STATELESSQ_H
#define MOH_STATELESSQ_H
#include "AnimCore.h"
#include "EAGLTransform.h"
namespace EAGLAnim {
class FnStatelessF3;
struct StatelessQ : public AnimMemoryMap {
    AttributeBlock *mAttributeBlock;
    unsigned short *mTimes;
    unsigned char *mBoneIdxs;
    FnStatelessF3 *mF3Ptr;
    unsigned short mNumKeys;
    unsigned char mNumBones, mNumConstBones;
    int GetNumFrames() const {
        if (!mTimes)
            return mNumKeys;
        return mTimes[mNumKeys - 2] + 1;
    }
};
} // namespace EAGLAnim
#endif
