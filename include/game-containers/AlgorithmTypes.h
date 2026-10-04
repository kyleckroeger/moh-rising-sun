// Scoped operation models, not complete historical class declarations.
// Member/helper names and source factoring are reconstruction choices; see docs/GameAlgorithms.md.
#ifndef MOH_GAME_ALGORITHM_TYPES_H
#define MOH_GAME_ALGORITHM_TYPES_H
#include "ContainerTypes.h"
struct ScoreInfo {
    unsigned char unresolved_00[12] __attribute__((aligned(4)));
    int field_0c;
    unsigned char unresolved_10[4];
    bool operator<(const ScoreInfo &other) const { return field_0c > other.field_0c; }
};
class SortByTeamAndScore {
    int field_00;

  public:
    bool operator()(const HandlerLeaderboard::Player &a,
                    const HandlerLeaderboard::Player &b) const {
        if (field_00 >= 0 && a.field_18 != b.field_18)
            return a.field_18 == field_00;
        if (a.field_10 != b.field_10)
            return a.field_10 > b.field_10;
        if (a.field_0c != b.field_0c)
            return a.field_0c < b.field_0c;
        if (a.field_1c != b.field_1c)
            return a.field_1c < b.field_1c;
        return a.field_14 < b.field_14;
    }
};
#include "../game/CMatrix.h"
struct DrawCommand {
    unsigned int field_00, field_04;
    unsigned char unresolved_08[24];
    bool operator<(const DrawCommand &other) const {
        if (field_00 < other.field_00)
            return true;
        if (field_00 > other.field_00)
            return false;
        return field_04 < other.field_04;
    }
};
struct DrawCommandData {
    CMatrix field_00;
    DrawCommand field_40;
    DrawCommandData(const DrawCommandData &other) {
        field_00 = other.field_00;
        field_40 = other.field_40;
    }
    bool operator<(const DrawCommandData &other) const { return field_40 < other.field_40; }
};
typedef char ScoreInfoStorageCheck[sizeof(ScoreInfo) == 20 ? 1 : -1];
typedef char DrawCommandStorageCheck[sizeof(DrawCommand) == 32 ? 1 : -1];
typedef char DrawCommandDataStorageCheck[sizeof(DrawCommandData) == 96 ? 1 : -1];
#endif
