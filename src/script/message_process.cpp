#include "ScriptRuntime.h"
void BSMessageProcessSingleMessageInQueue() {
    BSMessageQueueEntryView *entry = &g_MessageQueue[g_MessageQueueHead];
    g_MessageQueueHead = g_MessageQueueHead + 1 < 1024 ? g_MessageQueueHead + 1 : 0;
    if (entry->receiverIdentity == entry->receiver->queueIdentity) {
        BSMessageListView *current = FindCurrentMessageListEntry(entry->thread, entry->message);
        int execute = 0;
        if (current) {
            if (current->stateId != entry->stateId) execute = (current->handler->message->flags >> 1) & 1;
            else execute = 1;
        }
        if (execute) BSMessageExecuteHandler(current->handler, entry->sender, entry->target, entry->nativeObject, entry->context);
    }
}
