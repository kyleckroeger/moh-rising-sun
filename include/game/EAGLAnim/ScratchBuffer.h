// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_SCRATCHBUFFER_H
#define MOH_SCRATCHBUFFER_H
#pragma interface
#include "AnimCore.h"
namespace EAGLAnim {
class ScratchBuffer {
  public:
    void FreeBuffer();
    void *GetBuffer() { return mBuffer; }
    static ScratchBuffer &GetScratchBuffer(int);

  private:
    void *mBuffer;
    unsigned int mSize;
    int mRefCount;
};
class ScratchBufferHelper {
  public:
    static ScratchBuffer mScratchBuffers[3];
};
} // namespace EAGLAnim
#endif
