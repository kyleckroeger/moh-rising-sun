// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnRawEventChannel.h"
namespace EAGLAnim {
void FnRawEventChannel::operator delete(void *p, unsigned int size) { EAGLInternal::EAGLFree(p, size); }
} // namespace EAGLAnim
