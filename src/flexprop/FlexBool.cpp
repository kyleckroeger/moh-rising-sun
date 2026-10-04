#include "FlexProp.h"
bool FlexProp::GetBool(int key) const {
    int *p = static_cast<int *>(GetDataPtr(key));
    if (p)
        return *p != 0;
    else {
        DebugMsg("WARNING: Couldn't find flexprop bool field %x\n", key);
        return false;
    }
}
