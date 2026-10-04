// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawEventChannel.h"
namespace EAGLAnim {
void RawEventChannel::Eval(float previousTime, float currentTime, int &currentIdx, float &cacheCurrentTime,
                           EventHandler **eventHandlers, void *extraData) {
    int numEvents = GetNumEvents();
    Event *events = GetEvents();
    int i;
    if (previousTime < cacheCurrentTime) {
        for (i = currentIdx; i >= 0; i--) {
            if (events[i].triggerTime <= previousTime) {
                break;
            }
        }
        currentIdx = i + 1;
    }
    for (i = currentIdx; i < numEvents; i++) {
        if (events[i].triggerTime > previousTime) {
            break;
        }
    }
    currentIdx = i;

    if (previousTime == currentTime) {
        if (i >= numEvents) {
            currentIdx = numEvents - 1;
        }
        for (i = currentIdx; i >= 0; i--) {
            if (events[i].triggerTime < currentTime) {
                break;
            }
        }
        currentIdx = i + 1;
    } else if (currentTime < previousTime) {
        for (; i < numEvents; i++) {
            EventHandler *eh = eventHandlers[events[i].eventId];
            if (eh)
                eh->HandleEvent(currentTime, events[i], extraData);
        }
        currentIdx = 0;
    }

    for (i = currentIdx; i < numEvents; i++) {
        if (events[i].triggerTime <= currentTime) {
            EventHandler *eh = eventHandlers[events[i].eventId];
            if (eh)
                eh->HandleEvent(currentTime, events[i], extraData);
        } else {
            break;
        }
    }

    currentIdx = i;

    if (i >= numEvents) {
        currentIdx = numEvents - 1;
    }

    cacheCurrentTime = currentTime;
}

}; // namespace EAGLAnim
