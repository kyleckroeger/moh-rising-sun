// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "ObserverTypes.h"
void ISubject::AddObserver(IObserver *observer) {
    if (observer)
        observer->link.InsertAfter(&field_04.next);
}
