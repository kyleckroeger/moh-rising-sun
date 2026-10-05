#include "ScriptRuntime.h"
BSMessageListView *FindCurrentMessageListEntry(BSMachineThread_struct *thread, BSCode_MessageHandlerEntry_struct *message) {
    BSMessageListView *entry = thread->messages;
    while (entry) {
        BSCode_MessageHandlerEntry_struct *current = entry->handler->message;
        if (current->flags & 4) return 0;
        if (current->messageId == message->messageId && current->matchValue == message->matchValue && current->matchKind == message->matchKind)
            return entry;
        entry = entry->next;
    }
    return 0;
}
