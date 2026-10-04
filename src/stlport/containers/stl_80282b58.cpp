// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "TargetTypes.h"
namespace _STL {
template void __introsort_loop<TargetInfo *, TargetInfo, int, _STL::less<TargetInfo> >(
    TargetInfo *, TargetInfo *, TargetInfo *, int, _STL::less<TargetInfo>);
}
