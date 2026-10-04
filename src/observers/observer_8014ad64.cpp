// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
IObserver::~IObserver() { Assign(0); }
