// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ResourceTypes.h"
typedef _STL::vector<EALA::Character::MatrixBlend::SubTaskInfo,
                     _STL::allocator<EALA::Character::MatrixBlend::SubTaskInfo> >
    TargetContainer;
template void TargetContainer::reserve(unsigned int);
