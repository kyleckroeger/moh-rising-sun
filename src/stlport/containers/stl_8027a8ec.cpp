// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <vector>
namespace EAGL { class TAR; }

typedef _STL::vector<EAGL::TAR*, _STL::allocator<EAGL::TAR* > > TargetContainer;
template TargetContainer& TargetContainer::operator=(TargetContainer const&);
