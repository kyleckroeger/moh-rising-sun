// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
FnAnimMemoryMap *MemoryPoolManager::NewFnAnimAux(AnimMemoryMap *anim) {
    FnAnimMemoryMap *fnMemMap;
    switch (anim->GetType().GetType()) {
    case AnimTypeId::ANIM_STATELESSQ:
        fnMemMap = reinterpret_cast<FnAnimMemoryMap *>(reinterpret_cast<char *>(anim) - 24);
        break;
    case AnimTypeId::ANIM_STATELESSF3:
        fnMemMap = reinterpret_cast<FnAnimMemoryMap *>(reinterpret_cast<char *>(anim) - 24);
        break;
    case AnimTypeId::ANIM_POSEANIM:
        fnMemMap = reinterpret_cast<FnAnimMemoryMap *>(reinterpret_cast<char *>(anim) - 16);
        break;
    default:
        fnMemMap = static_cast<FnAnimMemoryMap *>(NewFnAnim(anim->GetType().GetType()));
        break;
    }
    fnMemMap->SetAnimMemoryMap(anim);
    return fnMemMap;
}
} // namespace EAGLAnim
