// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<unsigned char, _STL::pair<unsigned char const, PathFindingNetwork >, _STL::_Select1st<_STL::pair<unsigned char const, PathFindingNetwork > >, _STL::less<unsigned char >, _STL::allocator<_STL::pair<unsigned char const, PathFindingNetwork > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<unsigned char const, PathFindingNetwork > >*);
