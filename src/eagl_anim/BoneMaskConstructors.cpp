// AI-assisted reconstruction from pinned GR8E69; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
BoneMask::BoneMask(bool allOn) { SetAll(allOn); }
BoneMask::BoneMask(const BoneMask &bm) { *this = bm; }
} // namespace EAGLAnim
