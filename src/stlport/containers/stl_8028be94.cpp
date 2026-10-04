// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "AlgorithmTypes.h"
namespace _STL {
template ScoreInfo *__unguarded_partition<ScoreInfo *, ScoreInfo, _STL::less<ScoreInfo> >(
    ScoreInfo *, ScoreInfo *, ScoreInfo, _STL::less<ScoreInfo>);
}
