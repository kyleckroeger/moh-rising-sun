// AI-assisted reconstruction from pinned GR8E69; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
FnAnimMemoryMap::FnAnimMemoryMap() : mpAnim(0) {}
FnAnimMemoryMap::~FnAnimMemoryMap() {}
void FnAnimMemoryMap::SetAnimMemoryMap(AnimMemoryMap *anim) { mpAnim = anim; }
AnimMemoryMap *FnAnimMemoryMap::GetAnimMemoryMap() { return mpAnim; }
const AnimMemoryMap *FnAnimMemoryMap::GetAnimMemoryMap() const { return mpAnim; }
unsigned short FnAnimMemoryMap::GetTargetCheckSum() const { return mpAnim->GetTargetCheckSum(); }
} // namespace EAGLAnim
