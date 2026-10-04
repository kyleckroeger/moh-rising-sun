#pragma implementation "FnDeltaF1.h"
// EAGLAnim::FnDeltaF1 AI-assisted reconstruction for GR8E69 (original FnDeltaF1.cpp unit,
// .text 0x801f63e4..0x801f7cd8, vtable 0x802ecb78, constants 0x802bed78).
//
// Adapted from the public NFS Most Wanted decompilation (dbalatoni13/nfsmw, CC0,
// src/Speed/Indep/Src/EAGL4Anim/FnDeltaF1.cpp, commit 1f2cdd7996791c81a580b3f7b36b44d4f9f6719c).
// The Rising Sun library uses namespace EAGLAnim and an older FnAnim interface;
// each statement below was checked against the original instructions, see docs/Animation.md.
#include "EAGLAnim/FnDeltaF1.h"

namespace EAGLAnim {

bool FnDeltaF1::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (!mPrevValues) {
        InitBuffersAsRequired();
    }
    if (boneMask) {
        return EvalSQTMask(currTime, sqt, boneMask);
    }
    if (mBoneMask) {
        mPrevKey = -1;
        mNextKey = -1;
        mBoneMask = 0;
    }

    DeltaF1 *deltaF = reinterpret_cast<DeltaF1 *>(mpAnim);
    int floorTime = FloatToInt(currTime);
    int floorKey;
    if (!deltaF->mTimes) {
        if (floorTime < 0) {
            floorKey = 0;
        } else {
            if (floorTime >= deltaF->mNumFrames) {
                floorKey = deltaF->mNumFrames - 1;
            } else {
                floorKey = floorTime;
            }
        }
    } else if (floorTime < deltaF->mTimes[0]) {
        floorKey = 0;
    } else {
        int timeIndex;
        if (mPrevKey < 1) {
            timeIndex = 0;
        } else {
            timeIndex = mPrevKey - 1;
        }
        if (deltaF->mTimes[timeIndex] <= floorTime) {
            while (timeIndex < deltaF->mNumFrames - 2 && deltaF->mTimes[timeIndex + 1] <= floorTime) {
                timeIndex++;
            }
        } else {
            while (timeIndex > 0 && deltaF->mTimes[timeIndex] > floorTime) {
                timeIndex--;
            }
        }

        floorKey = timeIndex + 1;
    }
    unsigned int binLenPower = deltaF->GetBinLengthPower();
    unsigned int binLenModMask = deltaF->GetBinLengthModMask();

    int floorBinIdx = floorKey >> binLenPower;
    int floorDeltaIdx = floorKey & binLenModMask;
    int prevBinIdx = mPrevKey >> binLenPower;
    int prevDeltaIdx;
    unsigned char *binData = deltaF->GetBin(floorBinIdx);
    unsigned short *binPhys = deltaF->GetPhysical(binData);
    unsigned char *binDelta;
    int frameSize = deltaF->GetFrameDeltaSize();

    bool preventReverse = floorKey < mPrevKey && !IsReverseDeltaSumEnabled();
    if (floorKey == mNextKey) {
        float *tempBuf = mPrevValues;
        mPrevValues = mNextValues;
        mNextValues = tempBuf;
        prevDeltaIdx = floorDeltaIdx;
        mPrevKey = mNextKey;
        mNextKey = -1;
    } else if (mPrevKey == -1 || floorBinIdx != prevBinIdx || floorDeltaIdx == 0 || preventReverse) {
        for (int idof = 0; idof < deltaF->GetNumBones(); idof++) {
            mPrevValues[idof] = deltaF->UnQuantizePhysical(mMinRangesf[idof], binPhys[idof]);
        }
        prevDeltaIdx = 0;
    } else {
        prevDeltaIdx = mPrevKey & binLenModMask;
    }

    if (prevDeltaIdx < floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx);
        for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                mPrevValues[ibone] += deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta[ibone]);
            }
            binDelta += frameSize;
        }
    } else if (prevDeltaIdx > floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx - 1);
        for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                mPrevValues[ibone] -= deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta[ibone]);
            }
            binDelta -= frameSize;
        }
    }
    mPrevKey = floorKey;

    int ceilKey = floorKey + 1;
    float scale = 1.0f;
    bool lerpReqd;

    if (!deltaF->mTimes) {
        lerpReqd = currTime != floorTime;
        if (lerpReqd) {
            scale = currTime - floorTime;
        }
    } else if (floorKey == 0) {
        lerpReqd = currTime != 0.0f;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaF->mTimes[floorKey]);
            scale = currTime / ceilKeyTime;
        }
    } else {
        float floorKeyTime = deltaF->mTimes[floorKey - 1];
        lerpReqd = currTime != floorKeyTime;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaF->mTimes[floorKey]);
            scale = (currTime - floorKeyTime) / (ceilKeyTime - floorKeyTime);
        }
    }

    unsigned short *dofIndices = deltaF->GetDofIndices();

    if (lerpReqd && floorKey < deltaF->mNumFrames - 1) {
        int ceilBinIdx = ceilKey >> binLenPower;
        float ceil;
        binData = deltaF->GetBin(ceilBinIdx);
        binPhys = deltaF->GetPhysical(binData);
        if (ceilKey == mNextKey) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                sqt[*dofIndices] = mPrevValues[ibone] + (mNextValues[ibone] - mPrevValues[ibone]) * scale;
                dofIndices++;
            }
        } else {
            if (ceilBinIdx != floorBinIdx) {
                for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++, dofIndices++) {
                    mNextValues[ibone] = deltaF->UnQuantizePhysical(mMinRangesf[ibone], binPhys[ibone]);
                    sqt[*dofIndices] = mPrevValues[ibone] + (mNextValues[ibone] - mPrevValues[ibone]) * scale;
                }
            } else {
                int ceilDeltaIdx = floorDeltaIdx;
                binDelta = deltaF->GetDelta(binData, ceilDeltaIdx);
                for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                    ceil = deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta[ibone]);
                    mNextValues[ibone] = mPrevValues[ibone] + ceil;
                    sqt[*dofIndices] = mPrevValues[ibone] + (mNextValues[ibone] - mPrevValues[ibone]) * scale;
                    dofIndices++;
                }
            }
            mNextKey = ceilKey;
        }
    } else {
        for (int idof = 0; idof < deltaF->GetNumBones(); idof++) {
            sqt[dofIndices[idof]] = mPrevValues[idof];
        }
    }
    if (deltaF->mNumConstBones != 0) {
        unsigned short *constBoneIdxs = deltaF->GetConstBoneIdx();
        float *constPhys = deltaF->GetConstPhysical();

        for (int ibone = 0; ibone < deltaF->mNumConstBones; ibone++, constPhys++, constBoneIdxs++) {
            sqt[*constBoneIdxs] = *constPhys;
        }
    }

    return true;
}

bool FnDeltaF1::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mPrevKey = -1;
        mNextKey = -1;
        mBoneMask = boneMask;
    }

    DeltaF1 *deltaF = reinterpret_cast<DeltaF1 *>(mpAnim);
    int floorTime = FloatToInt(currTime);
    int floorKey;
    if (!deltaF->mTimes) {
        if (floorTime < 0) {
            floorKey = 0;
        } else {
            if (floorTime >= deltaF->mNumFrames) {
                floorKey = deltaF->mNumFrames - 1;
            } else {
                floorKey = floorTime;
            }
        }
    } else if (floorTime < deltaF->mTimes[0]) {
        floorKey = 0;
    } else {
        int timeIndex;
        if (mPrevKey < 1) {
            timeIndex = 0;
        } else {
            timeIndex = mPrevKey - 1;
        }
        if (deltaF->mTimes[timeIndex] <= floorTime) {
            while (timeIndex < deltaF->mNumFrames - 2 && deltaF->mTimes[timeIndex + 1] <= floorTime) {
                timeIndex++;
            }
        } else {
            while (timeIndex > 0 && deltaF->mTimes[timeIndex] > floorTime) {
                timeIndex--;
            }
        }

        floorKey = timeIndex + 1;
    }
    unsigned int binLenPower = deltaF->GetBinLengthPower();
    unsigned int binLenModMask = deltaF->GetBinLengthModMask();

    int floorBinIdx = floorKey >> binLenPower;
    int floorDeltaIdx = floorKey & binLenModMask;
    int prevBinIdx = mPrevKey >> binLenPower;
    int prevDeltaIdx;
    unsigned char *binData = deltaF->GetBin(floorBinIdx);
    unsigned short *binPhys = deltaF->GetPhysical(binData);
    unsigned char *binDelta;
    int frameSize = deltaF->GetFrameDeltaSize();

    unsigned short *dofIdxs = deltaF->GetDofIndices();
    unsigned char boneIdxs[80];
    int idof;
    // The original tests the bone count for zero before entering this loop and
    // re-tests `idof < count` at the bottom; a plain for-loop produces a signed
    // `0 < count` entry test instead (scratch variants gt/member/ne/uint/w1).
    idof = 0;
    if (deltaF->GetNumBones()) {
        do {
            boneIdxs[idof] = static_cast<unsigned char>(dofIdxs[idof] / 0xCu);
            idof++;
        } while (idof < deltaF->GetNumBones());
    }

    bool preventReverse = floorKey < mPrevKey && !IsReverseDeltaSumEnabled();
    if (floorKey == mNextKey) {
        float *tempBuf = mPrevValues;
        mPrevValues = mNextValues;
        mNextValues = tempBuf;
        prevDeltaIdx = floorDeltaIdx;
        mPrevKey = mNextKey;
        mNextKey = -1;
    } else if (mPrevKey == -1 || floorBinIdx != prevBinIdx || preventReverse) {
        for (idof = 0; idof < deltaF->GetNumBones(); idof++) {
            if (boneMask->GetBone(boneIdxs[idof])) {
                mPrevValues[idof] = deltaF->UnQuantizePhysical(mMinRangesf[idof], binPhys[idof]);
            }
        }
        prevDeltaIdx = 0;
    } else {
        prevDeltaIdx = mPrevKey & binLenModMask;
    }

    if (prevDeltaIdx < floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx);
        for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    mPrevValues[ibone] += deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta[ibone]);
                }
            }
            binDelta += frameSize;
        }
    } else if (prevDeltaIdx > floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx - 1);
        for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    mPrevValues[ibone] -= deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta[ibone]);
                }
            }
            binDelta -= frameSize;
        }
    }
    mPrevKey = floorKey;

    int ceilKey = floorKey + 1;
    float scale = 1.0f;
    bool lerpReqd;
    if (!deltaF->mTimes) {
        lerpReqd = currTime != floorTime;
        if (lerpReqd) {
            scale = currTime - floorTime;
        }
    } else if (floorKey == 0) {
        lerpReqd = currTime != 0.0f;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaF->mTimes[floorKey]);
            scale = currTime / ceilKeyTime;
        }
    } else {
        float floorKeyTime = deltaF->mTimes[floorKey - 1];
        lerpReqd = currTime != floorKeyTime;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaF->mTimes[floorKey]);
            scale = (currTime - floorKeyTime) / (ceilKeyTime - floorKeyTime);
        }
    }

    unsigned short *dofIndices = deltaF->GetDofIndices();

    if (lerpReqd && floorKey < deltaF->mNumFrames - 1) {
        int ceilBinIdx = ceilKey >> binLenPower;
        float ceil;
        binData = deltaF->GetBin(ceilBinIdx);
        binPhys = deltaF->GetPhysical(binData);
        if (ceilKey == mNextKey) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    sqt[*dofIndices] = mPrevValues[ibone] + (mNextValues[ibone] - mPrevValues[ibone]) * scale;
                }
                dofIndices++;
            }
        } else {
            if (ceilBinIdx != floorBinIdx) {
                for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        mNextValues[ibone] = deltaF->UnQuantizePhysical(mMinRangesf[ibone], binPhys[ibone]);
                        sqt[*dofIndices] =
                            mPrevValues[ibone] + (mNextValues[ibone] - mPrevValues[ibone]) * scale;
                    }
                    dofIndices++;
                }
            } else {
                int ceilDeltaIdx = floorDeltaIdx;
                binDelta = deltaF->GetDelta(binData, ceilDeltaIdx);
                for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        ceil = deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta[ibone]);
                        mNextValues[ibone] = mPrevValues[ibone] + ceil;
                        sqt[*dofIndices] =
                            mPrevValues[ibone] + (mNextValues[ibone] - mPrevValues[ibone]) * scale;
                    }
                    dofIndices++;
                }
            }
            mNextKey = ceilKey;
        }
    } else {
        for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                sqt[dofIndices[ibone]] = mPrevValues[ibone];
            }
        }
    }
    if (deltaF->mNumConstBones != 0) {
        unsigned short *constBoneIdxs = deltaF->GetConstBoneIdx();
        float *constPhys = deltaF->GetConstPhysical();
        unsigned short dofIndex;
        unsigned short boneIndex;

        for (int ibone = 0; ibone < deltaF->mNumConstBones; ibone++) {
            dofIndex = *constBoneIdxs++;
            boneIndex = dofIndex / 0xC;
            if (boneMask->GetBone(boneIndex)) {
                sqt[dofIndex] = *constPhys;
                constPhys++;
            } else {
                constPhys++;
            }
        }
    }

    return true;
}

void FnDeltaF1::InitBuffersAsRequired() {
    DeltaF1 *deltaF = reinterpret_cast<DeltaF1 *>(mpAnim);

    if (deltaF->GetNumBones() != 0) {
        mPrevValues = reinterpret_cast<float *>(
            MemoryPoolManager::NewBlock(deltaF->GetNumBones() * sizeof(*mPrevValues)));
        mPrevVBlock = mPrevValues;

        mNextValues = reinterpret_cast<float *>(
            MemoryPoolManager::NewBlock(deltaF->GetNumBones() * sizeof(*mNextValues)));
        mNextVBlock = mNextValues;

        mMinRangesf = reinterpret_cast<DeltaF1MinRange *>(
            MemoryPoolManager::NewBlock(deltaF->GetNumBones() * sizeof(*mMinRangesf)));

        DeltaF1::DofInfo *dofInfo = deltaF->GetDofInfo();

        for (int b = 0; b < deltaF->GetNumBones(); b++) {
            mMinRangesf[b].mPhysMin = dofInfo->mPhysMin;

            mMinRangesf[b].mPhysRange = dofInfo->mPhysRange;
            mMinRangesf[b].mPhysRange *= 1.5259022e-5f;

            mMinRangesf[b].mDeltaMin =
                2 * (dofInfo->mQuantMin * 1.5259022e-5f) * dofInfo->mPhysRange - dofInfo->mPhysRange;

            mMinRangesf[b].mDeltaRange = 2 * (dofInfo->mQuantRange * 1.5259022e-5f) * dofInfo->mPhysRange;
            mMinRangesf[b].mDeltaRange *= 0.003921569f;

            dofInfo++;
        }
    }
}

} // namespace EAGLAnim
