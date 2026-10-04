#include "FlexProp.h"
bool FlexProp::IsFieldValid(const char *key) const { return IsFieldValid(GetStringCRC(key)); }
int FlexProp::GetInt(const char *key) const { return GetInt(GetStringCRC(key)); }
int FlexProp::GetEnum(const char *key) const { return GetEnum(GetStringCRC(key)); }
float FlexProp::GetFloat(const char *key) const { return GetFloat(GetStringCRC(key)); }
const char *FlexProp::GetString(const char *key) const { return GetString(GetStringCRC(key)); }
bool FlexProp::GetBool(const char *key) const { return GetBool(GetStringCRC(key)); }
FlexPropList *FlexProp::GetList(const char *key) const { return GetList(GetStringCRC(key)); }
