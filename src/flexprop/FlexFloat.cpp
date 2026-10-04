#include "FlexProp.h"
float FlexProp::GetFloat(int key) const {
    float *p = static_cast<float *>(GetDataPtr(key));
    if (p)
        return *p;
    else {
        DebugMsg("WARNING: Couldn't find flexprop float field %x\n", key);
        return 0.0f;
    }
}
