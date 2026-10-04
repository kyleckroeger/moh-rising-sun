// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class IDestructible;

typedef _STL::_Rb_tree<IDestructible*, _STL::pair<IDestructible* const, int >, _STL::_Select1st<_STL::pair<IDestructible* const, int > >, _STL::less<IDestructible* >, _STL::allocator<_STL::pair<IDestructible* const, int > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<IDestructible* const, int > const&, _STL::_Rb_tree_node_base*);
