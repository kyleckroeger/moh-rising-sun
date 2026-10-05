#include "ScriptTimers.h"

BSTimerEvent_struct *BSGetFreeTimerEvent() {
    BSTimerEvent_struct *event = g_pFreeTimerEventHead;
    g_pFreeTimerEventHead = event->next;
    return event;
}
