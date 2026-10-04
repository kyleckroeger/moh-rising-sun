// Reconstructed single-axis quaternion format; target layouts and attributed reference names
// are documented in docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_DELTASINGLEQ_H
#define MOH_DELTASINGLEQ_H
#include "AnimCore.h"
#include "EAGLTransform.h"
extern "C" float sqrtf(float);
extern "C" float cosf(float);
extern "C" float sinf(float);
namespace EAGLAnim {
static inline void SingleQFromEuler(const COORD3 &e, COORD4 &q) {
    float x = e.x * 0.5f, y = e.y * 0.5f, z = e.z * 0.5f;
    float cx = cosf(x), cy = cosf(y), cz = cosf(z);
    float sx = sinf(x), sy = sinf(y), sz = sinf(z);
    q.x = cy * (sx * cz) - sy * (cx * sz);
    q.y = cy * (sx * sz) + sy * (cx * cz);
    q.z = cy * (cx * sz) - sy * (sx * cz);
    q.w = cy * (cx * cz) + sy * (sx * sz);
}
static inline void LerpNormalizedQ(const COORD4 &a, const COORD4 &b, float scale, COORD4 &result) {
    COORD4 q;
    q.x = a.x + scale * (b.x - a.x);
    q.y = a.y + scale * (b.y - a.y);
    q.z = a.z + scale * (b.z - a.z);
    q.w = a.w + scale * (b.w - a.w);
    float factor = 1.0f / sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    q.x *= factor;
    q.y *= factor;
    q.z *= factor;
    q.w *= factor;
    result = q;
}
struct DeltaSingleQMinRangef {
    float mConst0, mConst1, mMin[2], mRange[2];
    unsigned char mIndex;
};
struct DeltaSingleQMinRange {
    unsigned short mConst0, mConst1, mMin[2], mRange[2], mIndex;
    void GetAlignment(float &c0, float &c1) {
        const float angleScale = 6.28318530717958647692f / 65535.0f;
        c0 = mConst0 * angleScale + (-3.14159265358979323846f);
        c1 = mConst1 * angleScale + (-3.14159265358979323846f);
    }
    void UnQuantize(DeltaSingleQMinRangef &f) {
        const float factor = 2.0f / 65535.0f;
        f.mMin[0] = mMin[0] * factor - 1.0f;
        f.mMin[1] = mMin[1] * factor - 1.0f;
        f.mRange[0] = mRange[0] * factor;
        f.mRange[1] = mRange[1] * factor;
        f.mIndex = mIndex;
        GetAlignment(f.mConst0, f.mConst1);
    }
};
struct DeltaSingleQPhysical {
    unsigned char mV, mW;
    void UnQuantize(int index, COORD4 &q) const {
        const float scale = 2.0f / 255.0f;
        q.x = q.y = q.z = 0.0f;
        if (index == 0)
            q.x = mV * scale - 1.0f;
        else if (index == 1)
            q.y = mV * scale - 1.0f;
        else
            q.z = mV * scale - 1.0f;
        q.w = mW * scale - 1.0f;
    }
};
struct DeltaSingleQDelta {
    unsigned char mV : 4, mW : 4;
    void UnQuantize(const DeltaSingleQMinRangef &f, COORD4 &q) {
        q.x = q.y = q.z = 0.0f;
        if (f.mIndex == 0)
            q.x = f.mRange[0] * (1.0f / 15.0f) * mV + f.mMin[0];
        else if (f.mIndex == 1)
            q.y = f.mRange[0] * (1.0f / 15.0f) * mV + f.mMin[0];
        else
            q.z = f.mRange[0] * (1.0f / 15.0f) * mV + f.mMin[0];
        q.w = f.mRange[1] * (1.0f / 15.0f) * mW + f.mMin[1];
    }
};
struct DeltaSingleQ : AnimMemoryMap {
    unsigned short mNumKeys;
    unsigned char mNumBones, mBinLengthPower;
    unsigned short *mTimes;
    unsigned char *mBoneIdxs;
    int GetNumFrames() const {
        if (!mTimes)
            return mNumKeys;
        else
            return mTimes[mNumKeys - 2] + 1;
    }
    unsigned char GetBinLengthPower() const { return mBinLengthPower; }
    unsigned int GetBinLength() const { return 1 << mBinLengthPower; }
    unsigned int GetBinLengthModMask() const { return 0x7fffffffu >> (31 - mBinLengthPower); }
    int GetBinSize() const { return AlignSize2(mNumBones * (GetBinLength() + 1)); }
    DeltaSingleQMinRange *GetMinRange() { return reinterpret_cast<DeltaSingleQMinRange *>(this + 1); }
    void GetArrays(DeltaSingleQMinRange *&ranges, unsigned char *&bins) {
        ranges = GetMinRange();
        bins = reinterpret_cast<unsigned char *>(ranges + mNumBones);
    }
    DeltaSingleQPhysical *GetPhysical(unsigned char *bins) {
        return reinterpret_cast<DeltaSingleQPhysical *>(bins);
    }
};
} // namespace EAGLAnim
#endif
