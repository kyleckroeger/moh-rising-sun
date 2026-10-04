// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class IDestructible;

typedef _STL::_Rb_tree<IDestructible*, _STL::pair<IDestructible* const, int >, _STL::_Select1st<_STL::pair<IDestructible* const, int > >, _STL::less<IDestructible* >, _STL::allocator<_STL::pair<IDestructible* const, int > > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(_STL::pair<IDestructible* const, int > const&);
