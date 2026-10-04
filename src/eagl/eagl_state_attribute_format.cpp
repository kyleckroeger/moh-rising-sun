// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::SetAttributeFormat(int format, GXAttr attr, GXCompCnt count, GXCompType type,
                                               unsigned char fractional) {
    GXSetVtxAttrFmt((GXVtxFmt)format, attr, count, type, fractional);
}
