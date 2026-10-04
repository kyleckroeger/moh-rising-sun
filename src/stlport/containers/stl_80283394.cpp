// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "TargetTypes.h"
namespace _STL {
template void __final_insertion_sort<TargetInfo *, _STL::less<TargetInfo> >(TargetInfo *,
                                                                            TargetInfo *,
                                                                            _STL::less<TargetInfo>);
}
