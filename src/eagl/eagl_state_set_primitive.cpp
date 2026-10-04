// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
bool GeoPrimState::SetPrimitiveType(PrimitiveType type) {
    if (type < 0 && type >= -4) {
        extension.primitive = PRIMITIVE_POINT_LIST;
        return false;
    }
    extension.primitive = type;
    return true;
}
