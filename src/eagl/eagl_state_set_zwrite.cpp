// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::SetZWritesEnable(bool enabled) {
    if (enabled)
        zWriteOverride = 1;
    else
        zWriteOverride = 2;
}
