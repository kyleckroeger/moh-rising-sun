// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
bool TAR::SwapShape(const SHAPE *shape) {
    if (!extension.InitFromShape(shape))
        return false;
    return true;
}
