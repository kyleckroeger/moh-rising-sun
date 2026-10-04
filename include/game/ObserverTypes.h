// AI-assisted scoped reconstruction; see docs/Observers.md.
#ifndef MOH_OBSERVERTYPES_H
#define MOH_OBSERVERTYPES_H
#pragma interface
enum ESubjectEvent { EventUnused = 0 };
class IObserver;
struct ObserverLinkBase {
    ObserverLinkBase *next;
    ObserverLinkBase **prev;
    ObserverLinkBase() : next(0), prev(0) {}
    void InsertAfter(ObserverLinkBase **head) {
        next = *head;
        prev = head;
        if (next)
            next->prev = &next;
        if (prev)
            *prev = this;
    }
    void Unlink() {
        if (prev)
            *prev = next;
        if (next)
            next->prev = prev;
        next = 0;
        prev = 0;
    }
};
struct ObserverLink : ObserverLinkBase {
    IObserver *owner;
    ObserverLink(IObserver *o) : owner(o) {}
};
#include "IDestructible.h"
struct ObserverHead {
    ObserverLinkBase *next;
    ObserverHead() : next(0) {}
};
class ISubject : public IDestructible {
  protected:
    ObserverHead field_04;

  public:
    virtual void MarkForDestruction(int);
    virtual ~ISubject();
    void AddObserver(IObserver *);
    void NotifyObservers(ESubjectEvent);
};
class IObserver : public ISubject {
    friend class ISubject;
    ObserverLink link;

  protected:
    ISubject *subject;

  public:
    IObserver() : link(this), subject(0) {}
    virtual ~IObserver();
    virtual void HandleEvent(ISubject *, ESubjectEvent);
    void Assign(ISubject *s) {
        link.Unlink();
        subject = s;
        if (s)
            s->AddObserver(this);
    }
};

#endif
