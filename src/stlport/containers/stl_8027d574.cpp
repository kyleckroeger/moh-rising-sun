// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<char const*, _STL::pair<char const* const, EALA::Character::Mesh::Geometry >, _STL::_Select1st<_STL::pair<char const* const, EALA::Character::Mesh::Geometry > >, EALA::Character::Mesh::StringCompare, _STL::allocator<_STL::pair<char const* const, EALA::Character::Mesh::Geometry > > > TargetContainer;
template TargetContainer::iterator TargetContainer::insert_unique(TargetContainer::iterator, _STL::pair<char const* const, EALA::Character::Mesh::Geometry > const&);
