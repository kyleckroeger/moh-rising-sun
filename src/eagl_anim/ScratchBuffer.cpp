// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/ScratchBuffer.h"
namespace EAGLAnim {
void ScratchBuffer::FreeBuffer() {
    --mRefCount;
    if (mBuffer && mRefCount <= 0) {
        EAGLInternal::EAGLFree(mBuffer, mSize);
        mBuffer = 0;
    }
}
ScratchBuffer &ScratchBuffer::GetScratchBuffer(int i) { return ScratchBufferHelper::mScratchBuffers[i]; }
} // namespace EAGLAnim
