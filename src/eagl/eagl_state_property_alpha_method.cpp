// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
bool GeoPrimState::SetAlphaTestMethod(AlphaTestMethod value) {
    extension.alphaMethod = value;
    return true;
}
