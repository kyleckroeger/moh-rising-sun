// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<int, _STL::pair<int const, _LocationTargetInfo >, _STL::_Select1st<_STL::pair<int const, _LocationTargetInfo > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, _LocationTargetInfo > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<int const, _LocationTargetInfo > const&, _STL::_Rb_tree_node_base*);
