#include "ScriptRuntime.h"

BSMessageRegistration_struct *BSMessageGetFreeRegistration() {
    BSMessageRegistration_struct *entry = g_MessageRegistrationFreeListHead;
    g_MessageRegistrationFreeListHead = BSGetMessageHandlerByIndex(entry->nextIndex);
    return entry;
}
