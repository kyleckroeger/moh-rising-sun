// AI-assisted reconstruction from pinned GR8E69; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
#include <string.h>
namespace EAGLAnim {
inline BoneMask::BoneMask(bool allOn) { SetAll(allOn); }
inline BoneMask::BoneMask(const BoneMask &bm) { *this = bm; }
void BoneMask::SetAll(bool on) {
    if (on)
        memset(mMask, 255, sizeof(mMask));
    else
        memset(mMask, 0, sizeof(mMask));
}
void BoneMask::SetBone(int index, bool on) {
    if (on)
        mMask[index >> 5] |= 1 << (index & 31);
    else
        mMask[index >> 5] &= ~(1 << (index & 31));
}
BoneMask BoneMask::operator~() const {
    BoneMask result(false);
    for (int i = 0; i < 8; i++)
        result.mMask[i] = ~mMask[i];
    return result;
}
bool BoneMask::operator==(const BoneMask &other) const {
    for (int i = 0; i < 8; i++)
        if (mMask[i] != other.mMask[i])
            return false;
    return true;
}
} // namespace EAGLAnim
