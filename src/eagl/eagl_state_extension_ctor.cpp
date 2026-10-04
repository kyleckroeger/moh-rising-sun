// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
EAGL::GeoPrimStateExtension::GeoPrimStateExtension() {
    primitive = PRIMITIVE_TRIANGLE_LIST;
    depthTest = DTM_LEQUAL;
    alphaCompare = 16;
    alphaMethod = ATM_GREATER;
    blend = ABM_OFF;
    transparency = TM_OPAQUE;
    cull = false;
    alphaTest = true;
    cullOverride = 0;
    alphaWrites = true;
    zWriteOverride = 0;
}
