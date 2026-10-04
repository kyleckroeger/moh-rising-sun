// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::vector<CFont::FontRef, _STL::allocator<CFont::FontRef > > TargetContainer;
template void TargetContainer::reserve(unsigned int);
