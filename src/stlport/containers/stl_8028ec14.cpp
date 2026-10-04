// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "AlgorithmTypes.h"
namespace _STL {
template HandlerLeaderboard::Player *
__unguarded_partition<HandlerLeaderboard::Player *, HandlerLeaderboard::Player, SortByTeamAndScore>(
    HandlerLeaderboard::Player *, HandlerLeaderboard::Player *, HandlerLeaderboard::Player,
    SortByTeamAndScore);
}
