// Scoped reconstructed interface of the DeltaF3 memory-mapped animation layout (GR8E69).
//
// Offsets read by the original FnDeltaF3 functions (including 0x801f330c):
//   DeltaF3 header: +4 dofIdxs*, +8 times*, +12 numFrames(u16), +14 numBones(u16),
//   +16 binLengthPower(u8), +17 numConstBones(u8); size 20 (DofInfo starts at +20).
//   DofInfo: 36 bytes = float physMin[3] @0, float physRange[3] @12,
//   u16 quantMin[3] @24, u16 quantRange[3] @30 (InitBuffersAsRequired strides 36).
//   DeltaF3MinRange: 48 bytes (NewBlock(numBones*48)); physMin @0, physRange @12,
//   deltaMin @24, deltaRange @36 (three floats each).
//   Value buffers: 16 bytes per bone (NewBlock(numBones*16)), three floats used.
// Names follow the attributed NFS Most Wanted EAGL4Anim reference (DeltaF3.h);
// no original symbol names these inline members. COORD3/COORD4 are original
// game type names (mangled in EAGLAnim FnDeltaQFast symbols as 6COORD4 and
// declared the same way by include/game/EAGLTransform.h); the reference uses
// UMath::Vector3/Vector4 for the same roles.
#ifndef MOH_EAGLANIM_DELTAF3_H
#define MOH_EAGLANIM_DELTAF3_H

#include "AnimCore.h"

#include "EAGLTransform.h"

namespace EAGLAnim {

struct DeltaF3MinRange {
    float mPhysMin[3];
    float mPhysRange[3];
    float mDeltaMin[3];
    float mDeltaRange[3];
};

struct DeltaF3 : public AnimMemoryMap {
    struct DofInfo {
        float mPhysMin[3];
        float mPhysRange[3];
        unsigned short mQuantMin[3];
        unsigned short mQuantRange[3];
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
        unsigned int result = 0x7FFFFFFFU >> (0x1F - mBinLengthPower);
        return result;
    }

    int GetFrameDeltaSize() const { return mNumBones * 3; }

    unsigned short *GetDofIndices() { return mDofIdxs; }

    int GetBinSize() const {
        return AlignSize2((mNumBones * 6) + ((GetBinLength() - 1) * GetFrameDeltaSize()));
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
        return &reinterpret_cast<unsigned char *>(
            &reinterpret_cast<unsigned short *>(binData)[mNumBones * 3])[frameIdx * GetFrameDeltaSize()];
    }

    void UnQuantizePhysical(const DeltaF3MinRange &dofInfo, const unsigned short *physFrame,
                            COORD3 &result) const {
        result.x = physFrame[0] * dofInfo.mPhysRange[0] + dofInfo.mPhysMin[0];
        result.y = physFrame[1] * dofInfo.mPhysRange[1] + dofInfo.mPhysMin[1];
        result.z = physFrame[2] * dofInfo.mPhysRange[2] + dofInfo.mPhysMin[2];
    }

    void UnQuantizeDelta(const DeltaF3MinRange &dofInfo, const unsigned char *deltaFrame, int dofIdx,
                         COORD3 &result) const {
        const unsigned char *deltaPtr = &deltaFrame[dofIdx * 3];
        result.x = deltaPtr[0] * dofInfo.mDeltaRange[0] + dofInfo.mDeltaMin[0];
        result.y = deltaPtr[1] * dofInfo.mDeltaRange[1] + dofInfo.mDeltaMin[1];
        result.z = deltaPtr[2] * dofInfo.mDeltaRange[2] + dofInfo.mDeltaMin[2];
    }

    unsigned short *GetConstBoneIdx() {
        const int binSize = GetBinSize();
        int numBins = mNumFrames >> GetBinLengthPower();
        unsigned char *s = &GetBin(0)[binSize * numBins];
        int r = mNumFrames & GetBinLengthModMask();

        if (r > 0) {
            s = reinterpret_cast<unsigned char *>(
                AlignSize2(reinterpret_cast<int>(s) + mNumBones * 6 + (r - 1) * GetFrameDeltaSize()));
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
