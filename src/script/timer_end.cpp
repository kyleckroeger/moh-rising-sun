#include "ScriptTimers.h"

void BSEndTimer() {
    g_pTimerHashTable = 0;
    g_pTimerEventArray = 0;
    g_pFreeTimerEventHead = 0;
}
