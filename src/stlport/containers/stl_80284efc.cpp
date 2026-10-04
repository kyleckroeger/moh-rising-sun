// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CScriptedGag;

typedef _STL::_Rb_tree<unsigned int, _STL::pair<unsigned int const, CScriptedGag* >, _STL::_Select1st<_STL::pair<unsigned int const, CScriptedGag* > >, _STL::less<unsigned int >, _STL::allocator<_STL::pair<unsigned int const, CScriptedGag* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<unsigned int const, CScriptedGag* > const&, _STL::_Rb_tree_node_base*);
