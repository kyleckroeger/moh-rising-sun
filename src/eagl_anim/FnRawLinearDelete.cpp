// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawLinearChannel.h"
namespace EAGLAnim {
void FnRawLinearChannel::operator delete(void *p, unsigned int n) { EAGLInternal::EAGLFree(p, n); }
} // namespace EAGLAnim
