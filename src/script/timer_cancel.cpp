#include "ScriptTimers.h"

void BSCancelTimerEvent(unsigned short event, BSObject *object) {
    BSTimerRemoveDuplicateTimerInstances(object, event, -1);
}
