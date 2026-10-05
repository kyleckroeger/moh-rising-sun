#include "ScriptTimers.h"

void BSTimerRemoveTimerEvent(BSTimerEvent_struct *event, BSTimerEvent_struct **link) {
    if (event->ownsEventMemory) BSEventDecrimentReferenceCount(event->context);
    *link = event->next;
    event->next = g_pFreeTimerEventHead;
    g_pFreeTimerEventHead = event;
}
