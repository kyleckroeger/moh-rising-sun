// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
void TARExtension::SetClampModeV(GCClampMode mode) {
    int size = ShapeHeader(shape).height;
    if ((size & (size - 1)) && mode != 0)
        mode = GCCM_CLAMP;
    clampV = (GXTexWrapMode)mode;
    GXInitTexObjWrapMode(&texture, clampU, clampV);
}
