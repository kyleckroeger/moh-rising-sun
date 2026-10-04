// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
namespace EALA { class EAGLLoader; }

typedef _STL::_Rb_tree<void const*, _STL::pair<void const* const, EALA::EAGLLoader* >, _STL::_Select1st<_STL::pair<void const* const, EALA::EAGLLoader* > >, _STL::less<void const* >, _STL::allocator<_STL::pair<void const* const, EALA::EAGLLoader* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<void const* const, EALA::EAGLLoader* > const&, _STL::_Rb_tree_node_base*);
