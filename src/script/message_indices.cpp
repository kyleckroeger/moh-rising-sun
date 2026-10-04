#include "ScriptRuntime.h"

BSMessageRegistration_struct *BSGetMessageHandlerByIndex(short index) {
    if (index == -1)
        return 0;
    return &g_MessageRegistrationFreeList[index];
}

short BSGetMessageHandlerIndex(BSMessageRegistration_struct *entry) {
    if (!entry)
        return -1;
    return entry - g_MessageRegistrationFreeList;
}
