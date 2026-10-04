// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
void CDestructorQueue::Add(IDestructible &object, int count) {
    CDestructorQueue *q = sm_pSingleton;
    if (q->entries.find(&object) != q->entries.end())
        DebugMsg("Object already added to destructor queue. Ignoring duplicate request.\n");
    else
        q->entries[&object] = count;
}
