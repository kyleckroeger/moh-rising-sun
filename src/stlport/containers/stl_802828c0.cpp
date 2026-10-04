// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "TargetTypes.h"
namespace _STL {
template TargetInfo *__unguarded_partition<TargetInfo *, TargetInfo, _STL::less<TargetInfo> >(
    TargetInfo *, TargetInfo *, TargetInfo, _STL::less<TargetInfo>);
}
