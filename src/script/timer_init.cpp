#include "ScriptTimers.h"

void BSInitTimer() {
    g_pBSTimerMemory = (unsigned char *)BSUtilGetMemory(BSTimerGetMemoryRequirements(), 0);
    g_pTimerHashTable = (BSTimerBucketView *)g_pBSTimerMemory;
    g_pBSTimerMemoryOffset = 120 * sizeof(BSTimerBucketView);
    for (int i = 0; i < 120; ++i) {
        g_pTimerHashTable[i].head = 0;
        g_pTimerHashTable[i].tail = 0;
    }
    g_pTimerEventArray = (BSTimerEvent_struct *)(g_pBSTimerMemory + g_pBSTimerMemoryOffset);
    g_pBSTimerMemoryOffset += 256 * sizeof(BSTimerEvent_struct);
    g_pFreeTimerEventHead = g_pTimerEventArray;
    for (int i = 254; i >= 0; --i) g_pTimerEventArray[i].next = &g_pTimerEventArray[i + 1];
    g_pTimerEventArray[255].next = 0;
    g_LastHashIndex = 0;
    g_CurrentTime = 0.0f;
}
