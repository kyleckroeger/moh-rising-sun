// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
void CDestructorQueue::Init() {
    sm_pSingleton = new ("source/scene/destructor_queue.cpp:38") CDestructorQueue;
}
