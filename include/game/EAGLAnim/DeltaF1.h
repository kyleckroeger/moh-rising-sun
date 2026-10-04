// Scoped reconstructed interface of the DeltaF1 memory-mapped animation layout.
//
// Every offset below is read directly by the original FnDeltaF1 functions:
//   +4  unsigned short *mDofIdxs      (lwz 4;  dof indices, divided by 12 for bone)
//   +8  unsigned short *mTimes        (lwz 8;  key times, or null for uniform frames)
//   +12 unsigned short  mNumFrames    (lhz 12)
//   +14 unsigned short  mNumBones     (lhz 14)
//   +16 unsigned char   mBinLengthPower (lbz 16)
//   +17 unsigned char   mNumConstBones  (lbz 17)
//   size 20: per-bone DofInfo records (12 bytes: float, float, ushort, ushort)
//   start at offset 20, followed by bins of (numBones*2 + (binLen-1)*numBones)
//   bytes aligned to 2, then constant bone indices (2 bytes each) and 4-aligned
//   constant values.
// Field and method names follow the attributed NFS Most Wanted EAGL4Anim
// reference (DeltaF1.h); no original symbol names these inline members.
#ifndef MOH_EAGLANIM_DELTAF1_H
#define MOH_EAGLANIM_DELTAF1_H

#include "AnimCore.h"

namespace EAGLAnim {

// 16-byte per-bone unquantization record built by FnDeltaF1::InitBuffersAsRequired.
struct DeltaF1MinRange {
    float mPhysMin;
    float mPhysRange;
    float mDeltaMin;
    float mDeltaRange;
};

struct DeltaF1 : public AnimMemoryMap {
    struct DofInfo {
        float mPhysMin;
        float mPhysRange;
        unsigned short mQuantMin;
        unsigned short mQuantRange;
    };

    unsigned short GetNumFrames() const {
        if (!mTimes) {
            return mNumFrames;
        } else {
            return mTimes[mNumFrames - 2] + 1;
        }
    }

    unsigned short GetNumBones() const { return mNumBones; }

    unsigned int GetBinLength() const { return 1 << mBinLengthPower; }

    unsigned char GetBinLengthPower() const { return mBinLengthPower; }

    unsigned int GetBinLengthModMask() const {
        unsigned int result = 0x7FFFFFFFU >> (31 - mBinLengthPower);
        return result;
    }

    int GetFrameDeltaSize() const { return mNumBones; }

    unsigned short *GetDofIndices() { return mDofIdxs; }

    int GetBinSize() const {
        return AlignSize2((mNumBones * 2) + ((GetBinLength() - 1) * GetFrameDeltaSize()));
    }

    DofInfo *GetDofInfo() {
        unsigned char *memBytes = reinterpret_cast<unsigned char *>(&this[1]);
        return reinterpret_cast<DofInfo *>(memBytes);
    }

    unsigned char *GetBin(int binIdx) {
        const int bs = GetBinSize();
        unsigned char *memPos = &reinterpret_cast<unsigned char *>(GetDofInfo())[mNumBones * sizeof(DofInfo)];
        return &memPos[binIdx * bs];
    }

    unsigned short *GetPhysical(unsigned char *binData) {
        return reinterpret_cast<unsigned short *>(binData);
    }

    unsigned char *GetDelta(unsigned char *binData, int frameIdx) {
        return &(&binData[mNumBones * 2])[frameIdx * GetFrameDeltaSize()];
    }

    float UnQuantizePhysical(const DeltaF1MinRange &dofInfo, unsigned short physFrame) const {
        return physFrame * dofInfo.mPhysRange + dofInfo.mPhysMin;
    }

    float UnQuantizeDelta(const DeltaF1MinRange &dofInfo, unsigned char deltaFrame) const {
        return deltaFrame * dofInfo.mDeltaRange + dofInfo.mDeltaMin;
    }

    unsigned short *GetConstBoneIdx() {
        const int binSize = GetBinSize();
        int numBins = mNumFrames >> GetBinLengthPower();
        unsigned char *s = &GetBin(0)[binSize * numBins];
        int r = mNumFrames & GetBinLengthModMask();

        if (r > 0) {
            s = reinterpret_cast<unsigned char *>(
                AlignSize2(reinterpret_cast<int>(s) + mNumBones * 2 + (r - 1) * GetFrameDeltaSize()));
        }
        if (mNumBones == 0) {
            s = reinterpret_cast<unsigned char *>(AlignSize2(reinterpret_cast<int>(s)));
        }

        return reinterpret_cast<unsigned short *>(s);
    }

    float *GetConstPhysical() {
        return reinterpret_cast<float *>(
            AlignSize4(reinterpret_cast<int>(&GetConstBoneIdx()[mNumConstBones])));
    }

    unsigned short *mDofIdxs;
    unsigned short *mTimes;
    unsigned short mNumFrames;
    unsigned short mNumBones;
    unsigned char mBinLengthPower;
    unsigned char mNumConstBones;
    unsigned char mPadding[2];
};

} // namespace EAGLAnim

#endif
