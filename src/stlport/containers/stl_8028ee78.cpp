// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "AlgorithmTypes.h"
namespace _STL {
template void
__introsort_loop<HandlerLeaderboard::Player *, HandlerLeaderboard::Player, int, SortByTeamAndScore>(
    HandlerLeaderboard::Player *, HandlerLeaderboard::Player *, HandlerLeaderboard::Player *, int,
    SortByTeamAndScore);
}
