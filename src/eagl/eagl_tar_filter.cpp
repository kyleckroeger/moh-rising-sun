// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
bool TAR::SetFilterMode(FilterMode mode) {
    if (mode != -1) {
        extension.filterMode = mode;
        return true;
    }
    return false;
}
