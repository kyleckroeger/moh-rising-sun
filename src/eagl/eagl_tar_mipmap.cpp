// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
bool TAR::SetMIPMAPMode(MIPMAPMode mode) {
    extension.mipmapMode = mode;
    return true;
}
