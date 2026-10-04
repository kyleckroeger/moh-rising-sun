#include "FlexProp.h"
int FlexProp::GetFieldType(const char *key) const { return GetFieldType(GetStringCRC(key)); }
int FlexProp::GetFieldType(int key) const {
    FlexPropField *field = format->GetField(key);
    if (field)
        return field->type;
    else
        return 0;
}
const char *FlexProp::GetClassName() const { return format->description->name; }
