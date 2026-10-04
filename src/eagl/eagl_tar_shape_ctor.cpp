// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
TAR::TAR(const SHAPE *shape) {
    if ((ShapeHeader(shape).width & (ShapeHeader(shape).width - 1)) ||
        (ShapeHeader(shape).height & (ShapeHeader(shape).height - 1)))
        extension.clampV = extension.clampU = GX_CLAMP;
    else
        extension.clampV = extension.clampU = GX_REPEAT;
    extension.InitFromShape(shape);
}
