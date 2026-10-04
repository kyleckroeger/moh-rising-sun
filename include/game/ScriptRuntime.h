#ifndef GAME_SCRIPT_RUNTIME_H
#define GAME_SCRIPT_RUNTIME_H

#include "ScriptMachine.h"

// Scoped GR8E69 runtime views. Names for fields and View types are descriptive.
// Only the documented accesses/strides are established; these are not complete
// allocation types or on-disc formats. Do not allocate native objects from them.

// Method-only interface: the native TriggerObject layout remains unknown.
class TriggerObject {
public:
    int GetLegacyField(int) const;
};

// NEXTSTATE establishes this array stride, but does not inspect its fields.
struct BSCode_MessageHandlerEntry_struct {
    unsigned char unknown00[12];
};

struct BSMessageRegistration_struct {
    unsigned char unknown00[13];
    unsigned char flags; // +0x0d
};

struct BSEventView {
    unsigned int code;
    unsigned short eventNumber;
    unsigned char unknown06;
    unsigned char flags;
};

struct BSStateView {
    unsigned int code;
    BSEventView *events;
    BSCode_MessageHandlerEntry_struct *messages;
    unsigned short parent;
    unsigned char depth;
    unsigned char messageCount;
    unsigned char eventCount;
};

// The 28-byte stride is independently used by BSFindAnyEventHandler.
struct BSLevelView {
    unsigned char unknown00[8];
    BSStateView **states;
    unsigned short stateCount;
    unsigned char unknown0e[14];
};

struct BSClass_struct {
    BSLevelView *levels;
    unsigned char unknown04[84];
    unsigned short levelCount; // +0x58
    unsigned char unknown5a[6];
    int *sharedValues; // +0x60
};

struct BSObject {
    BSClass_struct *scriptClass;
    unsigned char unknown04[4];
    TriggerObject *nativeObject;
};

struct BSMessageListView {
    BSMessageRegistration_struct *handler;
    BSMessageRegistration_struct *registration;
    BSMessageListView *next;
    unsigned short depth;
    unsigned short stateId;
};

struct BSMachineThread_struct {
    unsigned char unknown00[16];
    BSStateView *state; // +0x10
    BSLevelView *level;
    BSMessageListView *messages;
    unsigned short stateId;
    unsigned short flags;
};

extern BSObject *g_pBSObject;
extern BSMachineThread_struct *g_pbmtThread;
extern BSStateView **g_ppsteStateEntries;
extern BSMessageListView *g_MessageListHead;

extern BSMessageListView *BSMachineGetFreeMessageList();
extern BSMessageRegistration_struct *BSRegisterMessage(
    BSCode_MessageHandlerEntry_struct *, BSObject *, unsigned short,
    BSMachineThread_struct *, BSMessageRegistration_struct **);
extern void BSMessageRemoveHandler(BSMessageRegistration_struct *);

#endif
