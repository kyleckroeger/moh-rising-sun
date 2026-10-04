// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawLinearChannel.h"
namespace EAGLAnim {
void FnRawLinearChannel::Eval(float, float time, float *output) {
    GetRawLinearChannel()->Eval(time, output, mInterp);
}
} // namespace EAGLAnim
