// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_RAW_POSE_CHANNEL_H
#define MOH_RAW_POSE_CHANNEL_H
#pragma interface
#include "AnimCore.h"
namespace EAGLAnim {
void EulF3(float *&, float *);
void QuatF4(float *&, float *);
void TranF3(float *&, float *);
void EulF3Interp(float, float *&, float *&, float *);
void QuatF4Interp(float, float *&, float *&, float *);
void TranF3Interp(float, float *&, float *&, float *);
class RawPoseChannel : public AnimMemoryMap {
  public:
    enum ChannelType { QUAT = 0, EUL = 1, TRAN = 2 };
    int GetNumFrames() const { return mNumFrames; }
    int GetSigSize() const { return mSigSize; }
    int *GetNonInterpSig() { return reinterpret_cast<int *>(this + 1); }
    int *GetInterpSig() { return GetNonInterpSig() + mSigSize; }
    float *GetAnimData() { return reinterpret_cast<float *>(GetInterpSig() + mSigSize); }
    float *GetFrame(int frame) { return GetAnimData() + frame * mFrameSize; }
    static void InitAnimMemoryMap(AnimMemoryMap *);
    void Eval(float, float *, bool, const BoneMask *);
    void EvalFrame(int, float *, const BoneMask *);

  private:
    int mSigSize;
    int mFrameSize;
    int mNumFrames;
};
class FnRawPoseChannel : public FnAnimMemoryMap {
  public:
    virtual ~FnRawPoseChannel();
    static void operator delete(void *, unsigned int);
    virtual bool GetLength(float &) const;
    virtual void Eval(float, float, float *);
    virtual bool EvalSQT(float, float *, const BoneMask *);

  private:
    bool mInterp;
};
} // namespace EAGLAnim
#endif
