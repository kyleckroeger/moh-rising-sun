// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class Checkpoint_t;

typedef _STL::_Rb_tree<int, _STL::pair<int const, Checkpoint_t* >, _STL::_Select1st<_STL::pair<int const, Checkpoint_t* > >, _STL::less<int >, _STL::allocator<_STL::pair<int const, Checkpoint_t* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::insert_unique(TargetContainer::iterator, _STL::pair<int const, Checkpoint_t* > const&);
