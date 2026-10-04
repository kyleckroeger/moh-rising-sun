// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCycle.h"
namespace EAGLAnim {
void FnCycle::operator delete(void *p, unsigned int n) { EAGLInternal::EAGLFree(p, n); }
} // namespace EAGLAnim
