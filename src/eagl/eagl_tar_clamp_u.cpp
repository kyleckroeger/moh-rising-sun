// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
void TARExtension::SetClampModeU(GCClampMode mode) {
    int size = ShapeHeader(shape).width;
    if ((size & (size - 1)) && mode != 0)
        mode = GCCM_CLAMP;
    clampU = (GXTexWrapMode)mode;
    GXInitTexObjWrapMode(&texture, clampU, clampV);
}
