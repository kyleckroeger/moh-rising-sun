#include "ScriptTimers.h"

void BSTimerRemoveDuplicateTimerInstances(BSObject *object, unsigned short number, int delay) {
    int first = (int)g_CurrentTime + 1;
    int last = first + 120;
    int bucket = g_LastHashIndex;
    for (int i = first; i <= last; ++i) {
        if (++bucket == 120) bucket = 0;
        BSTimerBucketView *list = g_pTimerHashTable + bucket;
        BSTimerEvent_struct *event = list->head;
        BSTimerEvent_struct **link = &list->head;
        BSTimerEvent_struct *tail = 0;
        while (event) {
            BSTimerEvent_struct *next = event->next;
            if (event->object == object && event->eventNumber == number) {
                BSTimerRemoveTimerEvent(event, link);
            } else {
                tail = event;
                link = &tail->next;
            }
            event = next;
        }
        g_pTimerHashTable[bucket].tail = tail;
    }
}
