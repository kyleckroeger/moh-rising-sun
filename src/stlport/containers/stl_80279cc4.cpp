// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CParticleDef;

typedef _STL::_Rb_tree<unsigned int, _STL::pair<unsigned int const, CParticleDef* >, _STL::_Select1st<_STL::pair<unsigned int const, CParticleDef* > >, _STL::less<unsigned int >, _STL::allocator<_STL::pair<unsigned int const, CParticleDef* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<unsigned int const, CParticleDef* > const&, _STL::_Rb_tree_node_base*);
