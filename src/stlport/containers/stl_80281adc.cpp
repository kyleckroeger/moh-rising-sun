// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class CPlayerWeaponObject;

typedef _STL::_Rb_tree<int, _STL::pair<int const, CPlayerWeaponObject* >, _STL::_Select1st<_STL::pair<int const, CPlayerWeaponObject* > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, CPlayerWeaponObject* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::insert_unique(TargetContainer::iterator, _STL::pair<int const, CPlayerWeaponObject* > const&);
