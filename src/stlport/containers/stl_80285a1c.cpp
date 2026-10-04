// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include "ContainerTypes.h"
typedef _STL::list<PathFindingNetworkLink, _STL::allocator<PathFindingNetworkLink> > TargetList;
template TargetList& TargetList::operator=(const TargetList&);
