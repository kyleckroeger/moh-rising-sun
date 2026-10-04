// Target-verified layout and attributed reference names; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_EAGLANIM_SKELETON_H
#define MOH_EAGLANIM_SKELETON_H
#pragma interface
#include "AnimCore.h"
#include "EAGLTransform.h"
namespace EAGLAnim {
// Reconstructed names follow the CC0 nfsmw reference; layout from GR8E69 accesses.
struct BoneData {
    COORD3 mS;
    int mParentIdx;
    COORD4 mQ;
    COORD3 mT;
    int mLeftRightIdx;
    EAGL::Transform mInvBaseMatrix;
};
// Only observed serialized sixteen-byte header; BoneData records follow it.
class SkeletonData {
  public:
    int GetNumBones() const { return mNumBones; }
    BoneData *GetBoneData() { return reinterpret_cast<BoneData *>(this + 1); }
    const BoneData *GetBoneData() const { return reinterpret_cast<const BoneData *>(this + 1); }
    const float *GetInvBoneScales() const { return mInvBoneScales; }

  private:
    unsigned short unknown_0, unknown_2, unknown_4, unknown_6;
    int mNumBones;
    float *mInvBoneScales;
};
extern void (*MatrixMultiply)(EAGL::Transform *, const EAGL::Transform *, const EAGL::Transform *);
class Skeleton : public SkeletonData {
  public:
    void MirrorPose(float *, float *, bool, const BoneMask *);
    void PoseSQTToLocal(float *, EAGL::Transform *, BoneMask *);
    void PoseLocalToGlobal(EAGL::Transform *, EAGL::Transform *, BoneMask *);
    void PoseSQTToGlobal(float *, EAGL::Transform *, BoneMask *);
    void GetStillPose(float *, const BoneMask *) const;
    void PoseGlobalToSkin(EAGL::Transform *, EAGL::Transform *, BoneMask *);
};
} // namespace EAGLAnim
#endif
