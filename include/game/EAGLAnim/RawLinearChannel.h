// CC0 nfsmw names and target-reconstructed methods; see docs/Animation.md.
#ifndef MOH_RAW_LINEAR_H
#define MOH_RAW_LINEAR_H
#pragma interface
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
class RawLinearChannel : public AnimMemoryMap {
  public:
    int GetNumDOFs() const { return mNumDOFs; }
    int GetNumFrames() const { return mNumFrames; }
    const unsigned short *GetDOFIndex() const { return reinterpret_cast<const unsigned short *>(this + 1); }
    const float *GetAnimData() const {
        return reinterpret_cast<const float *>(GetDOFIndex() + AlignSize2(mNumDOFs));
    }
    const float *GetFrame(int i) const { return GetAnimData() + i * mNumDOFs; }
    void EvalFrame(int frame, float *output) const {
        const float *data = GetFrame(frame);
        const unsigned short *indices = GetDOFIndex();
        for (int i = 0; i < GetNumDOFs(); i++)
            output[indices[i]] = data[i];
    }
    void EvalInterpFrame(float t, int frame0, int frame1, float *output) const {
        const float *data0 = GetFrame(frame0);
        const float *data1 = GetFrame(frame1);
        const unsigned short *indices = GetDOFIndex();
        for (int i = 0; i < GetNumDOFs(); i++)
            output[indices[i]] = t * (data1[i] - data0[i]) + data0[i];
    }
    void Eval(float time, float *output, bool interp) const {
        int frame = FloatToInt(time);
        if (frame < 0)
            EvalFrame(0, output);
        else if (frame >= GetNumFrames() - 1)
            EvalFrame(GetNumFrames() - 1, output);
        else {
            float t = time - frame;
            if (t != 0.0f && interp)
                EvalInterpFrame(t, frame, frame + 1, output);
            else
                EvalFrame(frame, output);
        }
    }

  private:
    unsigned short mNumDOFs, mNumFrames;
};
class FnRawLinearChannel : public FnAnimMemoryMap {
  public:
    virtual ~FnRawLinearChannel();
    static void operator delete(void *, unsigned int);
    const RawLinearChannel *GetRawLinearChannel() const {
        return reinterpret_cast<const RawLinearChannel *>(mpAnim);
    }
    virtual void Eval(float, float, float *);
    virtual bool GetLength(float &) const;

  private:
    bool mInterp;
};
} // namespace EAGLAnim
#endif
