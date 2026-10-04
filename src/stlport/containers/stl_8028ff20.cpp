// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<StaticMesh*, _STL::pair<StaticMesh* const, _STL::vector<AnimShape, _STL::allocator<AnimShape > > >, _STL::_Select1st<_STL::pair<StaticMesh* const, _STL::vector<AnimShape, _STL::allocator<AnimShape > > > >, _STL::less<StaticMesh* >, _STL::allocator<_STL::pair<StaticMesh* const, _STL::vector<AnimShape, _STL::allocator<AnimShape > > > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<StaticMesh* const, _STL::vector<AnimShape, _STL::allocator<AnimShape > > > >*);
