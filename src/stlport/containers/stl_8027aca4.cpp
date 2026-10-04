// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <vector>
namespace EAGL { class Model; }

typedef _STL::vector<EAGL::Model*, _STL::allocator<EAGL::Model* > > TargetContainer;
template void TargetContainer::reserve(unsigned int);
