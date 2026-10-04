// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CStaticObject;

typedef _STL::_Rb_tree<int, _STL::pair<int const, CStaticObject* >, _STL::_Select1st<_STL::pair<int const, CStaticObject* > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, CStaticObject* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<int const, CStaticObject* > const&, _STL::_Rb_tree_node_base*);
