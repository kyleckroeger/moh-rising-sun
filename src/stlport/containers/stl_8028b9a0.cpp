// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "AlgorithmTypes.h"
namespace _STL {
template void
__adjust_heap<ScoreInfo *, int, ScoreInfo, _STL::less<ScoreInfo> >(ScoreInfo *, int, int, ScoreInfo,
                                                                   _STL::less<ScoreInfo>);
}
