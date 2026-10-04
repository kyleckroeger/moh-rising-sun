// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ResourceTypes.h"
typedef _STL::vector<EALA::Resource::Pointer<EALA::EAGLLoader>,
                     _STL::allocator<EALA::Resource::Pointer<EALA::EAGLLoader> > >
    TargetContainer;
template void TargetContainer::reserve(unsigned int);
