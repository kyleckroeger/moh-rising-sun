// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
void CDestructorQueue::Execute() {
    CDestructorQueue *q = sm_pSingleton;
    _STL::map<IDestructible *, int>::iterator it = q->entries.begin(), end = q->entries.end();
    while (it != end) {
        IDestructible *object = it->first;
        int count = it->second - 1;
        if (count <= 0) {
            object->Destroy();
            q->entries.erase(it++);
        } else {
            q->entries[object] = count;
            ++it;
        }
    }
}
