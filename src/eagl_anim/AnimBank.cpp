// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimBank.h"
#include "EAGLAnim/CompoundChannel.h"
namespace EAGLAnim {
FnAnimMemoryMap *AnimBank::NewFnAnim(AnimMemoryMap *animMem) {
    FnAnimMemoryMap *fnStatelessQ;
    FnAnimMemoryMap *fnStatelessF3;
    FnAnimMemoryMap *fnPoseAnim;
    void *block;
    FnCompoundChannel *fnComp;
    switch (animMem->GetType().GetType()) {
    case AnimTypeId::ANIM_STATELESSQ:
        fnStatelessQ = reinterpret_cast<FnAnimMemoryMap *>(reinterpret_cast<char *>(animMem) - 24);
        fnStatelessQ->SetAnimMemoryMap(animMem);
        return fnStatelessQ;
    case AnimTypeId::ANIM_STATELESSF3:
        fnStatelessF3 = reinterpret_cast<FnAnimMemoryMap *>(reinterpret_cast<char *>(animMem) - 24);
        fnStatelessF3->SetAnimMemoryMap(animMem);
        return fnStatelessF3;
    case AnimTypeId::ANIM_POSEANIM:
        fnPoseAnim = reinterpret_cast<FnAnimMemoryMap *>(reinterpret_cast<char *>(animMem) - 16);
        fnPoseAnim->SetAnimMemoryMap(animMem);
        return fnPoseAnim;
    case AnimTypeId::ANIM_COMPOUND:
        block = MemoryPoolManager::NewBlockByIdx(AnimTypeId::ANIM_COMPOUND);
        fnComp = new (block) FnCompoundChannel();
        fnComp->SetAnimMemoryMap(animMem);
        return fnComp;
    default:
        return 0;
    }
}
void AnimBank::Constructor(void *ptr, EAGL::DynamicLoader *, const char *) {
    AnimBank *bank = reinterpret_cast<AnimBank *>(ptr);
    for (int i = bank->GetNumAnims() - 1; i >= 0; i--)
        MemoryPoolManager::InitAnimMemoryMap(bank->GetAnim(i));
}
void AnimBank::Destructor(void *) {}
FnAnimMemoryMap *AnimBank::NewFnAnim(int i) const { return NewFnAnim(mAnims[i]); }
} // namespace EAGLAnim
