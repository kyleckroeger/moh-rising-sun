// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
void ISubject::MarkForDestruction(int count) {
    IDestructible::MarkForDestruction(count);
    NotifyObservers((ESubjectEvent)4);
}
