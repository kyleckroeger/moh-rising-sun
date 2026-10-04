// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::GetCurrentVertex(unsigned int &format, unsigned int &type) {
    format = gVertexFormat;
    type = gVertexDataType;
}
