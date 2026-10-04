// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CUIObject;

typedef _STL::_Rb_tree<CUIObject*, CUIObject*, _STL::_Identity<CUIObject* >, _STL::less<CUIObject* >, _STL::allocator<CUIObject* > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, CUIObject* const&, _STL::_Rb_tree_node_base*);
