#ifndef GAME_SCRIPT_RUNTIME_H
#define GAME_SCRIPT_RUNTIME_H

#include "ScriptMachine.h"

// Scoped GR8E69 runtime views. Names for fields and View types are descriptive.
// Only the documented accesses/strides are established; these are not complete
// native object types or on-disc formats. Use only the established extents.

// Method-only interface: the native TriggerObject layout remains unknown.
class TriggerObject {
public:
    int GetLegacyField(int) const;
};

// Twelve-byte runtime message entry; match fields retain descriptive names.
struct BSCode_MessageHandlerEntry_struct {
    unsigned int code;
    unsigned int matchValue;
    unsigned short messageId;
    unsigned char matchKind;
    unsigned char flags;
};

struct BSMessageRegistration_struct {
    short objectIndex;
    short nextIndex;
    short previousIndex;
    unsigned short stateId;
    BSCode_MessageHandlerEntry_struct *message;
    unsigned char threadIndex;
    unsigned char flags; // +0x0d
    unsigned char unknown0e[2];
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

struct BSMachineThread_struct;

struct BSObject {
    BSClass_struct *scriptClass;
    BSMachineThread_struct *threads;
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

extern BSMessageRegistration_struct **g_MessageRegistrationArray;
extern BSMessageRegistration_struct *g_MessageRegistrationFreeList;
extern BSMessageRegistration_struct *g_MessageRegistrationFreeListHead;
extern int g_iBSMessageRegistrationListSize;
extern BSMessageListView *g_MessageList;
extern unsigned char *g_pBSMachineMemory;
extern int g_pBSMachineMemoryOffset;

extern BSMessageRegistration_struct *BSMessageGetFreeRegistration();
extern BSMessageRegistration_struct *BSGetMessageHandlerByIndex(short);
extern short BSGetMessageHandlerIndex(BSMessageRegistration_struct *);
extern unsigned char BSGetThreadIndex(BSMachineThread_struct *, BSObject *);
extern BSMachineThread_struct *BSGetThreadByIndex(unsigned char, BSObject *);
extern BSObject *BSGetObjectByIndex(short);
extern short BSGetIndexByObject(BSObject *);

#endif
