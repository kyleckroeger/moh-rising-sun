#pragma interface
#ifndef MOH_COMPOUND_H
#define MOH_COMPOUND_H
#include "AnimCore.h"
namespace EAGLAnim {
struct CompoundChannel : public AnimMemoryMap {
    const AttributeBlock *GetAttributeBlock() const { return mAttributeBlock; }
    unsigned short GetNumChannels() const { return mNumChannels; }
    unsigned short GetNumFrames() const { return mNumFrames; }
    AnimMemoryMap **GetChannels() { return mChannels; }
    const AnimMemoryMap *const *GetChannels() const { return mChannels; }
    static void InitAnimMemoryMap(AnimMemoryMap *);
    AttributeBlock *mAttributeBlock;
    unsigned short mNumChannels, mNumFrames;
    AnimMemoryMap *mChannels[1];
};
class FnCompoundChannel : public FnAnimMemoryMap {
  public:
    FnCompoundChannel() : mChannels(0), mUseFPS(false), mFPS(0) { mType = AnimTypeId::ANIM_COMPOUND; }
    static void *operator new(unsigned int, void *p) { return p; }
#include "AnimAllocation.h"
    virtual ~FnCompoundChannel();
    virtual void SetAnimMemoryMap(AnimMemoryMap *);
    virtual bool EvalEvent(float, float, EventHandler **, void *);
    virtual bool EvalSQT(float, float *, const BoneMask *);
    virtual bool EvalPose(float, const PosePaletteBank *, float *);
    virtual bool EvalWeights(float, float *);
    virtual bool EvalVel2D(float, float *);
    virtual bool EvalState(float, State *);
    virtual bool FindTime(const StateTest &, float, float &);
    virtual bool EvalPhase(float, PhaseValue &);
    virtual const PhaseChan *GetPhaseChan();
    virtual void UseFPS(bool);
    virtual unsigned short GetTargetCheckSum() const;
    virtual bool GetLength(float &) const;
    virtual const AttributeBlock *GetAttributes() const;
    virtual void Eval(float, float, float *);
    virtual bool GetAnimatedBones(BoneMask *, int &, int &, int &);
    virtual bool GetAnimatedBonesAux(BoneMask *, int &, int &, int &);
    CompoundChannel *GetCompoundChannel() { return reinterpret_cast<CompoundChannel *>(mpAnim); }
    const CompoundChannel *GetCompoundChannel() const {
        return reinterpret_cast<const CompoundChannel *>(mpAnim);
    }

  protected:
    void InitSubChannels() {
        CompoundChannel *cchannel = reinterpret_cast<CompoundChannel *>(mpAnim);
        mChannels = reinterpret_cast<FnAnim **>(
            MemoryPoolManager::NewBlock(cchannel->GetNumChannels() * sizeof(*mChannels)));
        for (int i = cchannel->GetNumChannels() - 1; i >= 0; i--)
            mChannels[i] =
                reinterpret_cast<FnAnim *>(MemoryPoolManager::NewFnAnim(cchannel->GetChannels()[i]));
    }

  private:
    FnAnim **mChannels;
    bool mUseFPS;
    unsigned char mFPS;
};
} // namespace EAGLAnim
#endif
