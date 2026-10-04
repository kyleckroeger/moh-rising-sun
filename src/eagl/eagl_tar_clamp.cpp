// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
bool TAR::SetClampMode(ClampMode mode) {
    if (((ShapeHeader(extension.shape).width & (ShapeHeader(extension.shape).width - 1)) ||
         (ShapeHeader(extension.shape).height & (ShapeHeader(extension.shape).height - 1))) &&
        mode != 0)
        mode = CM_CLAMP;
    extension.clampU = extension.clampV = (GXTexWrapMode)mode;
    GXInitTexObjWrapMode(&extension.texture, extension.clampU, extension.clampV);
    return true;
}
