// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class BSObject;

typedef _STL::_Rb_tree<int, _STL::pair<int const, BSObject* >, _STL::_Select1st<_STL::pair<int const, BSObject* > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, BSObject* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<int const, BSObject* > const&, _STL::_Rb_tree_node_base*);
