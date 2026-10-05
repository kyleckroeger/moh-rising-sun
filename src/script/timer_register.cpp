#include "ScriptTimers.h"

void BSRegisterTimerEvent(int delay, unsigned short number, BSObject *object,
                         void *context, ETimerReplaceMethod replace) {
    switch (replace) {
    case TIMER_REPLACE_DUPLICATES:
        BSTimerRemoveDuplicateTimerInstances(object, number, delay);
        break;
    case TIMER_KEEP_EARLIER:
        if (!BSTimerRemoveLatterTimerInstances(object, number, delay)) {
            if (g_pMemBlockAllocator.DoWeOwnThisMemory(context))
                BSEventDecrimentReferenceCount(context);
            return;
        }
        break;
    case TIMER_APPEND:
        break;
    }
    int ownsEventMemory = 0;
    if (g_pMemBlockAllocator.DoWeOwnThisMemory(context)) ownsEventMemory = 1;
    if (ownsEventMemory) BSEventIncrimentReferenceCount(context);
    if (object->queueIdentity == 0xffffffff) {
        if (ownsEventMemory) BSEventDecrimentReferenceCount(context);
        return;
    }
    BSTimerEvent_struct *event = BSGetFreeTimerEvent();
    event->time = g_CurrentTime + delay;
    event->eventNumber = number;
    event->object = object;
    event->context = context;
    event->ownsEventMemory = ownsEventMemory;
    event->next = 0;
    int bucket = event->time % 120;
    if (g_pTimerHashTable[bucket].tail) g_pTimerHashTable[bucket].tail->next = event;
    g_pTimerHashTable[bucket].tail = event;
    if (!g_pTimerHashTable[bucket].head) g_pTimerHashTable[bucket].head = event;
}
