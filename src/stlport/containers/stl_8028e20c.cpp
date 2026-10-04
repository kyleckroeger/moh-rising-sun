// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::vector<HandlerLeaderboard::Player, _STL::allocator<HandlerLeaderboard::Player > > TargetContainer;
template void TargetContainer::reserve(unsigned int);
