#include "FlexProp.h"
int FlexProp::GetInt(int key) const {
    int *p = static_cast<int *>(GetDataPtr(key));
    if (p)
        return *p;
    else {
        DebugMsg("WARNING: Couldn't find flexprop int field %x\n", key);
        return 0;
    }
}
