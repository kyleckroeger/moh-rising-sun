#include "FlexProp.h"
extern FlexPropList emptyList __asm__("empty.1110");
FlexPropList *FlexProp::GetList(int key) const {
    FlexPropList **p = static_cast<FlexPropList **>(GetDataPtr(key));
    if (p)
        return *p;
    else {
        DebugMsg("WARNING: Couldn't find flexprop list field %x\n", key);
        return &emptyList;
    }
}
