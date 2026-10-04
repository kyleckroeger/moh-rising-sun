#include "FlexProp.h"
int FlexProp::GetEnum(int key) const {
    int *p = static_cast<int *>(GetDataPtr(key));
    if (p)
        return *p;
    else {
        DebugMsg("WARNING: Couldn't find flexprop enum field %x\n", key);
        return 0;
    }
}
