// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>


typedef _STL::_Rb_tree<void const*, _STL::pair<void const* const, unsigned int >, _STL::_Select1st<_STL::pair<void const* const, unsigned int > >, _STL::greater<void const* >, _STL::allocator<_STL::pair<void const* const, unsigned int > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<void const* const, unsigned int > const&, _STL::_Rb_tree_node_base*);
