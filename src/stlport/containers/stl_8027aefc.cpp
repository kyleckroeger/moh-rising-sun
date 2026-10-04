// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <vector>


typedef _STL::vector<void*, _STL::allocator<void* > > TargetContainer;
template void TargetContainer::reserve(unsigned int);
