// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
#include <stdio.h>
namespace EAGLAnim {
void RawPoseChannel::InitAnimMemoryMap(AnimMemoryMap *anim) {
    RawPoseChannel *channel = reinterpret_cast<RawPoseChannel *>(anim);
    int sigSize = channel->GetSigSize();
    int *sig = channel->GetNonInterpSig();
    int i;
    for (i = 0; i < sigSize;) {
        int count = *sig++;
        i++;
        for (int j = 0; j < count; j++, sig++) {
            i++;
            switch (*sig) {
            case EUL:
                *sig = reinterpret_cast<int>(EulF3);
                break;
            case QUAT:
                *sig = reinterpret_cast<int>(QuatF4);
                break;
            case TRAN:
                *sig = reinterpret_cast<int>(TranF3);
                break;
            default:
                printf("Bad signature channel type\n");
                break;
            }
        }
    }
    sig = channel->GetInterpSig();
    for (i = 0; i < sigSize;) {
        int count = *sig++;
        i++;
        for (int j = 0; j < count; j++, sig++) {
            i++;
            switch (*sig) {
            case EUL:
                *sig = reinterpret_cast<int>(EulF3Interp);
                break;
            case QUAT:
                *sig = reinterpret_cast<int>(QuatF4Interp);
                break;
            case TRAN:
                *sig = reinterpret_cast<int>(TranF3Interp);
                break;
            default:
                printf("Bad signature channel type\n");
                break;
            }
        }
    }
}

} // namespace EAGLAnim
