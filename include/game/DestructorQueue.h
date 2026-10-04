// Observed queue storage and allocation boundary; see docs/Observers.md.
#ifndef MOH_DESTRUCTOR_QUEUE_H
#define MOH_DESTRUCTOR_QUEUE_H
#include "ObserverTypes.h"
#include "../stlport-runtime/profile.h"
#include <map>
#include <new>
extern "C" void *DWI_alloc(const char *, int, int);
class CDestructorQueue {
    _STL::map<IDestructible *, int> entries;
    static CDestructorQueue *sm_pSingleton;

  public:
    CDestructorQueue();
    static void *operator new(unsigned int size, const char *label) {
        return DWI_alloc(label, size, 1024);
    }
    static void Init();
    static void Reset();
    static void Add(IDestructible &, int);
    static void Execute();
};
void DebugMsg(const char *, ...);

typedef char DestructorQueueStorageCheck[sizeof(CDestructorQueue) == 16 ? 1 : -1];
#endif
