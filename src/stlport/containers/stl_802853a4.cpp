// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CScriptedGag;

typedef _STL::_Rb_tree<unsigned int, _STL::pair<unsigned int const, CScriptedGag* >, _STL::_Select1st<_STL::pair<unsigned int const, CScriptedGag* > >, _STL::less<unsigned int >, _STL::allocator<_STL::pair<unsigned int const, CScriptedGag* > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<unsigned int const, CScriptedGag* > >*);
