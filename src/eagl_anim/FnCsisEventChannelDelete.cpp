// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/FnCsisEventChannel.h"
namespace EAGLAnim {
void FnCsisEventChannel::operator delete(void *p, unsigned int size) { EAGLInternal::EAGLFree(p, size); }
} // namespace EAGLAnim
