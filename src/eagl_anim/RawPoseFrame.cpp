// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
namespace EAGLAnim {
void RawPoseChannel::EvalFrame(int frame, float *outputPose, const BoneMask *boneMask) {
    int *sig = GetNonInterpSig();
    float *data = GetFrame(frame);
    int *sigEnd = GetInterpSig();
    float *pose = outputPose;
    typedef void (*EvalFunc)(float *&, float *);
    EvalFunc eval;
    int i, count;
    if (!boneMask) {
        while (sig < sigEnd) {
            count = *sig++;
            for (i = 0; i < count; i++) {
                eval = reinterpret_cast<EvalFunc>(*sig++);
                eval(data, pose + 4);
            }
            pose += 12;
        }
    } else {
        int bone = 0;
        while (sig < sigEnd) {
            count = *sig++;
            bool selected = boneMask->GetBone(bone);
            if (selected) {
                for (i = 0; i < count; i++) {
                    eval = reinterpret_cast<EvalFunc>(*sig++);
                    eval(data, pose + 4);
                }
            } else {
                for (i = 0; i < count; i++) {
                    eval = reinterpret_cast<EvalFunc>(*sig++);
                    if (eval == EulF3 || eval == TranF3)
                        data += 3;
                    else if (eval == QuatF4)
                        data += 4;
                }
            }
            pose += 12;
            bone++;
        }
    }
}
} // namespace EAGLAnim
