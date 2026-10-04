// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <vector>


typedef _STL::vector<int, _STL::allocator<int > > TargetContainer;
template void TargetContainer::reserve(unsigned int);
