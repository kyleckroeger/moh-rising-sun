// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
void MemoryPoolManager::ResetPoolAux() {
    gMemoryPoolFree = gMemoryPool;
    int i;
    for (i = 255; i >= 0; i--) {
        gSizeFreeList[i] = 0;
    }
    for (i = 25; i >= 0; i--) {
        gFreeList[i] = 0;
    }
}

void MemoryPoolManager::InitAux(unsigned int poolSize) {
    gMemoryPoolSize = poolSize;
    gMemoryPool = (char *)EAGLInternal::EAGLMalloc(poolSize, "EAGLAnim Memory Pool");
    gMaxIdx = 0;
    ResetPool();
}

void MemoryPoolManager::CleanupAux() { EAGLInternal::EAGLFree(gMemoryPool, gMemoryPoolSize); }

void MemoryPoolManager::DeleteBlockByIdxAux(unsigned short idx, void *ptr) {
    *reinterpret_cast<char **>(ptr) = gFreeList[idx];
    gFreeList[idx] = reinterpret_cast<char *>(ptr);
}

void MemoryPoolManager::DeleteBlockAux(void *ptr) {
    unsigned int idx = reinterpret_cast<int *>(ptr)[-1];

    *reinterpret_cast<char **>(ptr) = gSizeFreeList[idx];
    gSizeFreeList[idx] = reinterpret_cast<char *>(ptr);
}

void MemoryPoolManager::DeleteFnAnimAux(FnAnim *fnAnim) {
    if (fnAnim->GetType() == AnimTypeId::ANIM_STATELESSQ) {
        return;
    }
    if (fnAnim->GetType() == AnimTypeId::ANIM_STATELESSF3) {
        return;
    }
    if (fnAnim->GetType() == AnimTypeId::ANIM_POSEANIM) {
        return;
    }
    fnAnim->~FnAnim();
    DeleteBlockByIdx(fnAnim->GetType(), fnAnim);
}
} // namespace EAGLAnim
