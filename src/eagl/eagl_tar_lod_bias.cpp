// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
bool TAR::SetMIPMAPLODBias(float value) {
    bool okay = true;
    if (value > 3.99f) {
        value = 3.99f;
        okay = false;
    } else if (value < -4.0f) {
        value = -4.0f;
        okay = false;
    }
    extension.lodBias = value;
    return okay;
}
