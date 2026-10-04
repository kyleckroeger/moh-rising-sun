#ifndef GAME_FLEXPROP_H
#define GAME_FLEXPROP_H
#include "CMatrix.h"
#include "StringCRC.h"

// Scoped storage views reconstructed from GR8E69. Member names are descriptive;
// the serialized records have variable tails. See docs/FlexProp.md.
struct FlexPropField {
    int crc;
    int type;
    int offset;
};
// The class header is followed by count twelve-byte field records.
struct FlexPropClassView {
    const char *name;
    unsigned int unknown_4;
    FlexPropClassView *parent;
    unsigned int count;
    FlexPropField fields[0];
};
class FlexPropFormat {
  public:
    unsigned int unknown_0;
    FlexPropClassView *description;
    unsigned int unknown_8;
    float transform[12];
    FlexPropField *GetField(int);
};
struct FlexPropList;
class FlexProp {
  public:
    FlexPropFormat *format;
    void *GetDataPtr(int) const;
    int GetFieldOffset(int) const;
    void *GetData(int) const;
    bool IsFieldValid(int) const;
    bool IsFieldValid(const char *) const;
    int GetInt(int) const;
    int GetInt(const char *) const;
    int GetEnum(int) const;
    int GetEnum(const char *) const;
    float GetFloat(int) const;
    float GetFloat(const char *) const;
    bool GetBool(int) const;
    bool GetBool(const char *) const;
    const char *GetString(int) const;
    const char *GetString(const char *) const;
    FlexPropList *GetList(int) const;
    FlexPropList *GetList(const char *) const;
    int GetFieldType(int) const;
    int GetFieldType(const char *) const;
    void GetPosition(CVector3 &) const;
    void SetPositionZ(float);
    const char *GetClassName() const;
    bool IsA(const char *) const;
};
void DebugMsg(const char *, ...);
#endif
