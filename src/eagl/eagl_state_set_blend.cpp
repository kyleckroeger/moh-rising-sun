// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::SetBlendMode(GXBlendMode mode, GXBlendFactor source, GXBlendFactor destination,
                                         GXLogicOp logic) {
    blend = ABM_CUSTOM;
    customMode = mode;
    customSource = source;
    customDestination = destination;
    customLogic = logic;
}
