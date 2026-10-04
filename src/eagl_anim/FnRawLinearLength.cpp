// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawLinearChannel.h"
namespace EAGLAnim {
bool FnRawLinearChannel::GetLength(float &length) const {
    length = GetRawLinearChannel()->GetNumFrames();
    return true;
}
} // namespace EAGLAnim
