// Scoped target interface; reference names adapted from CC0 nfsmw.
// See docs/Animation.md and src/eagl_anim/NOTICE for evidence and limits.
#ifndef MOH_RAWSTATECHAN_H
#define MOH_RAWSTATECHAN_H
#pragma interface
#include "AnimCore.h"
namespace EAGLAnim {
struct RawStateChan : AnimMemoryMap {
    unsigned short mNumFrames, mNumKeys;
    unsigned char mNumFields, mKeySize;
    unsigned short mDecodeData[0];

    void GetDecodeData(int index, unsigned char &bits, unsigned char &bytes,
                       unsigned char &offset) const {
        unsigned short field = mDecodeData[index];
        bits = field >> 13;
        bytes = ((field >> 11) & 3) + 1;
        offset = field;
    }
};
class FnRawStateChan : public FnAnimMemoryMap {
  public:
#include "AnimAllocation.h"
    virtual ~FnRawStateChan() {}
    virtual bool GetLength(float &length) const {
        length = static_cast<float>(reinterpret_cast<RawStateChan *>(mpAnim)->mNumFrames);
        return true;
    }
    virtual void Eval(float, float time, float *dofs) {
        EvalState(time, reinterpret_cast<State *>(dofs));
    }
    void Decode(unsigned char *, unsigned char *) const;
    virtual bool EvalState(float, State *);
    virtual bool FindTime(const StateTest &, float, float &);
    int mKeyIdx;
};
} // namespace EAGLAnim
#endif
