// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > >, _STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, EALA::Character::SkeletonData::AttachPoint >, _STL::_Select1st<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, EALA::Character::SkeletonData::AttachPoint > >, EALA::Character::SkeletonData::Compare, _STL::allocator<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, EALA::Character::SkeletonData::AttachPoint > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, EALA::Character::SkeletonData::AttachPoint > >*);
