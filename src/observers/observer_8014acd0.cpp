// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
#include "DestructorQueue.h"
ISubject::~ISubject() { NotifyObservers((ESubjectEvent)8); }
