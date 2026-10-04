// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
void ISubject::NotifyObservers(ESubjectEvent e) {
    ObserverLinkBase *node = field_04.next;
    while (node) {
        IObserver *observer = static_cast<ObserverLink *>(node)->owner;
        ObserverLinkBase *next = node->next;
        if (observer)
            observer->HandleEvent(this, e);
        node = next;
    }
}
