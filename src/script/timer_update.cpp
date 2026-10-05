#include "ScriptTimers.h"

void BSUpdateTimer(float dt) {
    int previous = (int)g_CurrentTime;
    g_CurrentTime += dt;
    int current = (int)g_CurrentTime;
    for (int i = previous + 1; i <= current; ++i) {
        if (++g_LastHashIndex == 120) g_LastHashIndex = 0;
        BSTimerBucketView *list = g_pTimerHashTable + g_LastHashIndex;
        BSTimerEvent_struct *event = list->head;
        BSTimerEvent_struct **link = &list->head;
        BSTimerEvent_struct *tail = 0;
        while (event) {
            BSTimerEvent_struct *next = event->next;
            if (!(event->time > g_CurrentTime)) {
                BSObjectTriggerEvent(event->object, event->eventNumber, event->context, 0, false);
                BSTimerRemoveTimerEvent(event, link);
            } else {
                tail = event;
                link = &tail->next;
            }
            event = next;
        }
        g_pTimerHashTable[g_LastHashIndex].tail = tail;
    }
}
