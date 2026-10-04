// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
#include "EAGLAnim/ScratchBuffer.h"
namespace EAGLAnim {
FnRunBlender::~FnRunBlender() {
    if (mFnAnims[0])
        MemoryPoolManager::DeleteFnAnim(mFnAnims[0]);
    if (mFnAnims[1])
        MemoryPoolManager::DeleteFnAnim(mFnAnims[1]);
    if (mFnVelAnims[0])
        MemoryPoolManager::DeleteFnAnim(mFnVelAnims[0]);
    if (mFnVelAnims[1])
        MemoryPoolManager::DeleteFnAnim(mFnVelAnims[1]);
    if (mNumAnims)
        ScratchBuffer::GetScratchBuffer(0).FreeBuffer();
    if (mAnims)
        MemoryPoolManager::DeleteBlock(mAnims);
    if (mPhases)
        MemoryPoolManager::DeleteBlock(mPhases);
    if (mVels)
        MemoryPoolManager::DeleteBlock(mVels);
    if (mSpeed)
        MemoryPoolManager::DeleteBlock(mSpeed);
}
} // namespace EAGLAnim
