// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "TargetTypes.h"
namespace _STL {
template void __adjust_heap<TargetInfo *, int, TargetInfo, _STL::less<TargetInfo> >(
    TargetInfo *, int, int, TargetInfo, _STL::less<TargetInfo>);
}
