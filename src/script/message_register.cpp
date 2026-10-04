#include "ScriptRuntime.h"

BSMessageRegistration_struct *BSRegisterMessage(
    BSCode_MessageHandlerEntry_struct *message, BSObject *object, unsigned short state,
    BSMachineThread_struct *thread, BSMessageRegistration_struct **associated) {
    *associated = 0;
    if (message->flags & 1) {
        BSMessageRegistration_struct *entry = g_MessageRegistrationArray[message->messageId];
        while (entry) {
            BSObject *registeredObject = BSGetObjectByIndex(entry->objectIndex);
            BSCode_MessageHandlerEntry_struct *registeredMessage = entry->message;
            BSMachineThread_struct *registeredThread = BSGetThreadByIndex(entry->threadIndex, registeredObject);
            if (thread == registeredThread && message->matchValue == registeredMessage->matchValue
                    && message->matchKind == registeredMessage->matchKind) {
                if (message->code == registeredMessage->code)
                    return entry;
                entry->flags |= 2;
                *associated = entry;
                break;
            }
            entry = BSGetMessageHandlerByIndex(entry->nextIndex);
        }
    }
    BSMessageRegistration_struct *entry = BSMessageGetFreeRegistration();
    if (!entry)
        return 0;
    entry->objectIndex = BSGetIndexByObject(object);
    entry->threadIndex = BSGetThreadIndex(thread, object);
    entry->stateId = state;
    entry->flags = 0;
    entry->message = message;
    if (message->flags & 1) {
        entry->nextIndex = BSGetMessageHandlerIndex(g_MessageRegistrationArray[message->messageId]);
        entry->previousIndex = -1;
        g_MessageRegistrationArray[message->messageId] = entry;
        if (entry->nextIndex != -1) {
            BSMessageRegistration_struct *next = BSGetMessageHandlerByIndex(entry->nextIndex);
            next->previousIndex = BSGetMessageHandlerIndex(entry);
        }
    } else {
        entry->nextIndex = -1;
        entry->previousIndex = -1;
    }
    return entry;
}
