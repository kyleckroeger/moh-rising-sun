// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
bool GeoPrimState::SetTransparencyMethod(TransparencyMethod value) {
    extension.transparency = value;
    return true;
}
