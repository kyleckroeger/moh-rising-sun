// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::SetCullDirection(CullDirection direction) {
    if (direction == CD_CLOCKWISE)
        cullOverride = 2;
    else
        cullOverride = 1;
}
