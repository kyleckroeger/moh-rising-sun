// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<int, _STL::pair<int const, AnimShape::InitParams >, _STL::_Select1st<_STL::pair<int const, AnimShape::InitParams > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, AnimShape::InitParams > > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(_STL::pair<int const, AnimShape::InitParams > const&);
