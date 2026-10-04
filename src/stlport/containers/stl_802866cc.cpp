// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<unsigned short, _STL::pair<unsigned short const, BSMessageToEventRecord >, _STL::_Select1st<_STL::pair<unsigned short const, BSMessageToEventRecord > >, _STL::less<unsigned short >, _STL::allocator<_STL::pair<unsigned short const, BSMessageToEventRecord > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<unsigned short const, BSMessageToEventRecord > >*);
