#include "ScriptTimers.h"

void BSUnregisterTimerEvents(BSObject *object) {
    BSTimerRemoveTimerInstances(object);
}
