// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawPoseChannel.h"
namespace EAGLAnim {
FnRawPoseChannel::~FnRawPoseChannel() {}
void FnRawPoseChannel::operator delete(void *p, unsigned int size) { EAGLInternal::EAGLFree(p, size); }
bool FnRawPoseChannel::GetLength(float &length) const {
    length = static_cast<float>(reinterpret_cast<RawPoseChannel *>(mpAnim)->GetNumFrames());
    return true;
}
void FnRawPoseChannel::Eval(float, float currentTime, float *pose) {
    reinterpret_cast<RawPoseChannel *>(mpAnim)->Eval(currentTime, pose, mInterp, 0);
}
bool FnRawPoseChannel::EvalSQT(float currentTime, float *pose, const BoneMask *mask) {
    reinterpret_cast<RawPoseChannel *>(mpAnim)->Eval(currentTime, pose, mInterp, mask);
    return true;
}
} // namespace EAGLAnim
