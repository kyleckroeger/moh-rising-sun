#include "ScriptRuntime.h"
int BSDestroyThread(BSMachineThread_struct *thread) {
    BSMessageListView *entry = thread->messages;
    while (entry) {
        BSMessageListView *next = entry->next;
        BSMessageRemoveHandler(entry->handler);
        entry->next = g_MessageListHead;
        g_MessageListHead = entry;
        entry = next;
    }
    return 0;
}
