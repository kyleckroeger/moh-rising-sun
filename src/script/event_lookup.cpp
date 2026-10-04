#include "ScriptRuntime.h"

int BSFindAnyEventHandler(unsigned short event, BSClass_struct *scriptClass) {
    for (int level = 0; level < scriptClass->levelCount; ++level) {
        for (int state = 0; state < scriptClass->levels[level].stateCount; ++state) {
            BSStateView *entry = scriptClass->levels[level].states[state];
            if (entry) {
                for (int handler = 0; handler < entry->eventCount; ++handler) {
                    if (entry->events[handler].eventNumber == event)
                        return 1;
                }
            }
        }
    }
    return 0;
}
