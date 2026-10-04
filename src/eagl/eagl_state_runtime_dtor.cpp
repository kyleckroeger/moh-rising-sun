// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
#include "EAGLPropertyParser.h"
namespace EAGLInternal {
void RuntimeAllocGeoPrimStateDestructor(void *p, int) { delete (EAGL::GeoPrimState *)p; }
} // namespace EAGLInternal
