// Scoped target interface and attributed reference names; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_FNPOSEBLENDER_RUN_REFERENCE_H
#define MOH_FNPOSEBLENDER_RUN_REFERENCE_H
#pragma interface
#include "AnimCore.h"
namespace EAGLAnim {
// Static interface only, supported by its original symbol and callers.
// The instance layout and remaining methods are not reconstructed here.
class FnPoseBlender {
  public:
    static void Blend(int, float, const float *, const float *, float *, const BoneMask *);
};
} // namespace EAGLAnim
#endif
