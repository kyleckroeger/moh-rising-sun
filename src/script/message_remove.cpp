#include "ScriptRuntime.h"

void BSMessageRemoveHandler(BSMessageRegistration_struct *entry) {
    if (entry->message->flags & 1) {
        if (entry->previousIndex != -1)
            BSGetMessageHandlerByIndex(entry->previousIndex)->nextIndex = entry->nextIndex;
        else
            g_MessageRegistrationArray[entry->message->messageId] =
                BSGetMessageHandlerByIndex(entry->nextIndex);
        if (entry->nextIndex != -1)
            BSGetMessageHandlerByIndex(entry->nextIndex)->previousIndex = entry->previousIndex;
    }
    entry->nextIndex = BSGetMessageHandlerIndex(g_MessageRegistrationFreeListHead);
    g_MessageRegistrationFreeListHead = entry;
}
