// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::SetRenderContextStates() {
    bool clockwise = gCullClockwise, zWrites = gZWritesEnable;
    if (gCurrentState.cullOverride)
        clockwise = gCurrentState.cullOverride == 2;
    if (gCurrentState.zWriteOverride)
        zWrites = gCurrentState.zWriteOverride == 1;
    if (gCurrentState.cull)
        GXSetCullMode(clockwise ? GX_CULL_FRONT : GX_CULL_BACK);
    else
        GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, (GXCompare)gCurrentState.depthTest, zWrites ? GX_TRUE : GX_FALSE);
    GXSetColorUpdate(gColourWritesEnable ? GX_TRUE : GX_FALSE);
}
