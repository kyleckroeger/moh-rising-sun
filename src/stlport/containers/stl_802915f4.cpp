// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class IRegisterSerializable;

typedef _STL::_Rb_tree<int, _STL::pair<int const, IRegisterSerializable* >, _STL::_Select1st<_STL::pair<int const, IRegisterSerializable* > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, IRegisterSerializable* > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<int const, IRegisterSerializable* > >*);
