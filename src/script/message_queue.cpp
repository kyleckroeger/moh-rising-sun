#include "ScriptRuntime.h"
int BSMessageQueueMessage(BSObject *sender, BSObject *target, TriggerObject *nativeObject, BSMessageRegistration_struct *registration, void *context) {
    int next = g_MessageQueueTail + 1;
    if (next < 1024 ? g_MessageQueueHead == next : g_MessageQueueHead == 0)
        return 12;
    BSMessageQueueEntryView *entry = &g_MessageQueue[g_MessageQueueTail];
    BSObject *receiver = BSGetObjectByIndex(registration->objectIndex);
    entry->sender = sender;
    entry->target = target;
    entry->nativeObject = nativeObject;
    entry->receiver = receiver;
    entry->thread = BSGetThreadByIndex(registration->threadIndex, receiver);
    entry->context = context;
    entry->message = registration->message;
    entry->receiverIdentity = receiver->queueIdentity;
    entry->stateId = registration->stateId;
    g_MessageQueueTail = g_MessageQueueTail + 1 < 1024 ? g_MessageQueueTail + 1 : 0;
    return 0;
}
