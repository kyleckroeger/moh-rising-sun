// Scoped target interface and attributed reference names; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_PHASECHAN_H
#define MOH_PHASECHAN_H
#pragma interface
#include "AnimCore.h"
namespace EAGLAnim {
// Twelve-byte observed header, followed by format payload. Names from CC0 nfsmw.
struct PhaseChan : public AnimMemoryMap {
    bool StartWithRight() const { return (mFlag & 1) != 0; }
    bool FindMatchTime(const MatchPhaseInput &, float &) const;
    unsigned short mNumFrames, mStartTime;
    unsigned char mFlag, unknown_9, mCycles[2];
};
} // namespace EAGLAnim
#endif
