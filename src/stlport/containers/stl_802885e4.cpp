// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "AlgorithmTypes.h"
namespace _STL {
template void __adjust_heap<DrawCommandData *, int, DrawCommandData, _STL::less<DrawCommandData> >(
    DrawCommandData *, int, int, DrawCommandData, _STL::less<DrawCommandData>);
}
