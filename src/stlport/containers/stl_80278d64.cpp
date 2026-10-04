// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CUIObject;

typedef _STL::_Rb_tree<CUIObject*, CUIObject*, _STL::_Identity<CUIObject* >, _STL::less<CUIObject* >, _STL::allocator<CUIObject* > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<CUIObject* >*);
