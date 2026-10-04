// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<unsigned int, _STL::pair<unsigned int const, CProjectorSystemDef >, _STL::_Select1st<_STL::pair<unsigned int const, CProjectorSystemDef > >, _STL::less<unsigned int >, _STL::allocator<_STL::pair<unsigned int const, CProjectorSystemDef > > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(_STL::pair<unsigned int const, CProjectorSystemDef > const&);
