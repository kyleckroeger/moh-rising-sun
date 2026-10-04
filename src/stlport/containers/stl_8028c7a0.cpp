// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CUIObject;

typedef _STL::_Rb_tree<CUIObject*, CUIObject*, _STL::_Identity<CUIObject* >, _STL::less<CUIObject* >, _STL::allocator<CUIObject* > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(CUIObject* const&);
