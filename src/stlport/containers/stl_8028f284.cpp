// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include "AlgorithmTypes.h"
namespace _STL {
template void __unguarded_linear_insert<HandlerLeaderboard::Player *, HandlerLeaderboard::Player,
                                        SortByTeamAndScore>(HandlerLeaderboard::Player *,
                                                            HandlerLeaderboard::Player,
                                                            SortByTeamAndScore);
}
