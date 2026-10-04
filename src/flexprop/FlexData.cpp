#include "FlexProp.h"
void *FlexProp::GetDataPtr(int key) const {
    FlexPropFormat *f = format;
    FlexPropField *field = f->GetField(key);
    if (field)
        return reinterpret_cast<char *>(f) + 60 + field->offset;
    else
        return 0;
}
int FlexProp::GetFieldOffset(int key) const {
    FlexPropField *field = format->GetField(key);
    return field ? field->offset : -1;
}
void *FlexProp::GetData(int offset) const { return reinterpret_cast<char *>(format) + 60 + offset; }
