#pragma implementation "FnDeltaF3.h"
// EAGLAnim::FnDeltaF3 AI-assisted reconstruction for GR8E69 (original FnDeltaF3.cpp unit,
// .text 0x801f16a4..0x801f3854, vtable 0x802eca00, constants 0x802becd0).
//
// Adapted from the public NFS Most Wanted decompilation (dbalatoni13/nfsmw, CC0,
// src/Speed/Indep/Src/EAGL4Anim/FnDeltaF3.cpp, commit 1f2cdd7996791c81a580b3f7b36b44d4f9f6719c).
// Namespace changed to the original EAGLAnim; COORD3/COORD4 replace UMath vectors
// (see DeltaF3.h). Each statement is checked against the original instructions; see docs/Animation.md.
#include "EAGLAnim/FnDeltaF3.h"

namespace EAGLAnim {

bool FnDeltaF3::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
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

    DeltaF3 *deltaF = reinterpret_cast<DeltaF3 *>(mpAnim);
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
    COORD3 delta;
    // Shared counter scope preserves the original scheduling; see docs/Animation.md.
    int iframe;

    bool preventReverse = floorKey < mPrevKey && !IsReverseDeltaSumEnabled();
    if (floorKey == mNextKey) {
        COORD4 *tempBuf = mPrevValues;
        mPrevValues = mNextValues;
        mNextValues = tempBuf;
        prevDeltaIdx = floorDeltaIdx;
        mPrevKey = mNextKey;
        mNextKey = -1;
    } else if (mPrevKey == -1 || floorBinIdx != prevBinIdx || floorDeltaIdx == 0 || preventReverse) {
        for (int idof = 0; idof < deltaF->GetNumBones(); idof++) {
            deltaF->UnQuantizePhysical(mMinRangesf[idof], &binPhys[idof * 3],
                                       *reinterpret_cast<COORD3 *>(&mPrevValues[idof]));
        }
        prevDeltaIdx = 0;
    } else {
        prevDeltaIdx = mPrevKey & binLenModMask;
    }

    if (prevDeltaIdx < floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx);
        for (iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta, ibone, delta);
                mPrevValues[ibone].x += delta.x;
                mPrevValues[ibone].y += delta.y;
                mPrevValues[ibone].z += delta.z;
            }
            binDelta += frameSize;
        }
    } else if (prevDeltaIdx > floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx - 1);
        for (iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta, ibone, delta);
                mPrevValues[ibone].x -= delta.x;
                mPrevValues[ibone].y -= delta.y;
                mPrevValues[ibone].z -= delta.z;
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
        COORD3 ceil;
        binData = deltaF->GetBin(ceilBinIdx);
        binPhys = deltaF->GetPhysical(binData);
        if (ceilKey == mNextKey) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                sqt[*dofIndices + 0] =
                    mPrevValues[ibone].x + (mNextValues[ibone].x - mPrevValues[ibone].x) * scale;
                sqt[*dofIndices + 1] =
                    mPrevValues[ibone].y + (mNextValues[ibone].y - mPrevValues[ibone].y) * scale;
                sqt[*dofIndices + 2] =
                    mPrevValues[ibone].z + (mNextValues[ibone].z - mPrevValues[ibone].z) * scale;
                dofIndices++;
            }
        } else {
            if (ceilBinIdx != floorBinIdx) {
                // Preserve the original loop-update order; see docs/Animation.md.
                for (int ibone = 0; ibone < deltaF->GetNumBones(); dofIndices++, binPhys += 3, ibone++) {
                    deltaF->UnQuantizePhysical(mMinRangesf[ibone], binPhys,
                                               *reinterpret_cast<COORD3 *>(&mNextValues[ibone]));
                    sqt[*dofIndices + 0] =
                        mPrevValues[ibone].x + (mNextValues[ibone].x - mPrevValues[ibone].x) * scale;
                    sqt[*dofIndices + 1] =
                        mPrevValues[ibone].y + (mNextValues[ibone].y - mPrevValues[ibone].y) * scale;
                    sqt[*dofIndices + 2] =
                        mPrevValues[ibone].z + (mNextValues[ibone].z - mPrevValues[ibone].z) * scale;
                }
            } else {
                int ceilDeltaIdx = floorDeltaIdx;
                binDelta = deltaF->GetDelta(binData, ceilDeltaIdx);
                for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                    deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta, ibone, ceil);
                    mNextValues[ibone].x = mPrevValues[ibone].x + ceil.x;
                    mNextValues[ibone].y = mPrevValues[ibone].y + ceil.y;
                    mNextValues[ibone].z = mPrevValues[ibone].z + ceil.z;

                    sqt[*dofIndices + 0] =
                        mPrevValues[ibone].x + (mNextValues[ibone].x - mPrevValues[ibone].x) * scale;
                    sqt[*dofIndices + 1] =
                        mPrevValues[ibone].y + (mNextValues[ibone].y - mPrevValues[ibone].y) * scale;
                    sqt[*dofIndices + 2] =
                        mPrevValues[ibone].z + (mNextValues[ibone].z - mPrevValues[ibone].z) * scale;
                    dofIndices++;
                }
            }
            mNextKey = ceilKey;
        }
    } else {
        for (int idof = 0; idof < deltaF->GetNumBones(); idof++) {
            *reinterpret_cast<COORD3 *>(&sqt[dofIndices[idof]]) =
                *reinterpret_cast<COORD3 *>(&mPrevValues[idof]);
        }
    }
    if (deltaF->mNumConstBones != 0) {
        unsigned short *constBoneIdxs = deltaF->GetConstBoneIdx();
        float *constPhys = deltaF->GetConstPhysical();
        int index;

        for (int ibone = 0; ibone < deltaF->mNumConstBones; ibone++) {
            index = *constBoneIdxs++;
            sqt[index++] = *constPhys++;
            sqt[index++] = *constPhys++;
            sqt[index] = *constPhys++;
        }
    }

    return true;
}

bool FnDeltaF3::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mPrevKey = -1;
        mNextKey = -1;
        mBoneMask = boneMask;
    }

    DeltaF3 *deltaF = reinterpret_cast<DeltaF3 *>(mpAnim);
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
    COORD3 delta;

    unsigned short *dofIdxs = deltaF->GetDofIndices();
    unsigned char boneIdxs[80];
    int idof;
    // Same zero-test guarded do/while as the matched FnDeltaF1 unit.
    idof = 0;
    if (deltaF->GetNumBones()) {
        do {
            boneIdxs[idof] = static_cast<unsigned char>(dofIdxs[idof] / 0xCu);
            idof++;
        } while (idof < deltaF->GetNumBones());
    }

    bool preventReverse = floorKey < mPrevKey && !IsReverseDeltaSumEnabled();
    if (floorKey == mNextKey) {
        COORD4 *tempBuf = mPrevValues;
        mPrevValues = mNextValues;
        mNextValues = tempBuf;
        prevDeltaIdx = floorDeltaIdx;
        mPrevKey = mNextKey;
        mNextKey = -1;
    } else if (mPrevKey == -1 || floorBinIdx != prevBinIdx || preventReverse) {
        for (idof = 0; idof < deltaF->GetNumBones(); idof++) {
            if (boneMask->GetBone(boneIdxs[idof])) {
                deltaF->UnQuantizePhysical(mMinRangesf[idof], &binPhys[idof * 3],
                                           *reinterpret_cast<COORD3 *>(&mPrevValues[idof]));
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
                    deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta, ibone, delta);
                    mPrevValues[ibone].x += delta.x;
                    mPrevValues[ibone].y += delta.y;
                    mPrevValues[ibone].z += delta.z;
                }
            }
            binDelta += frameSize;
        }
    } else if (prevDeltaIdx > floorDeltaIdx) {
        binDelta = deltaF->GetDelta(binData, prevDeltaIdx - 1);
        for (int iframe = prevDeltaIdx - 1; iframe >= floorDeltaIdx; iframe--) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta, ibone, delta);
                    mPrevValues[ibone].x -= delta.x;
                    mPrevValues[ibone].y -= delta.y;
                    mPrevValues[ibone].z -= delta.z;
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
        COORD3 ceil;
        binData = deltaF->GetBin(ceilBinIdx);
        binPhys = deltaF->GetPhysical(binData);
        if (ceilKey == mNextKey) {
            for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    sqt[*dofIndices] =
                        mPrevValues[ibone].x + (mNextValues[ibone].x - mPrevValues[ibone].x) * scale;
                    sqt[*dofIndices + 1] =
                        mPrevValues[ibone].y + (mNextValues[ibone].y - mPrevValues[ibone].y) * scale;
                    sqt[*dofIndices + 2] =
                        mPrevValues[ibone].z + (mNextValues[ibone].z - mPrevValues[ibone].z) * scale;
                }
                dofIndices++;
            }
        } else {
            if (ceilBinIdx != floorBinIdx) {
                for (int ibone = 0; ibone < deltaF->GetNumBones(); dofIndices++, binPhys += 3, ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        deltaF->UnQuantizePhysical(mMinRangesf[ibone], binPhys,
                                                   *reinterpret_cast<COORD3 *>(&mNextValues[ibone]));
                        mNextKey = ceilKey;
                        sqt[*dofIndices] =
                            mPrevValues[ibone].x + (mNextValues[ibone].x - mPrevValues[ibone].x) * scale;
                        sqt[*dofIndices + 1] =
                            mPrevValues[ibone].y + (mNextValues[ibone].y - mPrevValues[ibone].y) * scale;
                        sqt[*dofIndices + 2] =
                            mPrevValues[ibone].z + (mNextValues[ibone].z - mPrevValues[ibone].z) * scale;
                    }
                }
            } else {
                int ceilDeltaIdx = floorDeltaIdx;
                binDelta = deltaF->GetDelta(binData, ceilDeltaIdx);
                for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
                    if (boneMask->GetBone(boneIdxs[ibone])) {
                        deltaF->UnQuantizeDelta(mMinRangesf[ibone], binDelta, ibone, ceil);
                        mNextKey = ceilKey;
                        mNextValues[ibone].x = mPrevValues[ibone].x + ceil.x;
                        mNextValues[ibone].y = mPrevValues[ibone].y + ceil.y;
                        mNextValues[ibone].z = mPrevValues[ibone].z + ceil.z;

                        sqt[*dofIndices] = mPrevValues[ibone].x + ceil.x * scale;
                        sqt[*dofIndices + 1] = mPrevValues[ibone].y + ceil.y * scale;
                        sqt[*dofIndices + 2] = mPrevValues[ibone].z + ceil.z * scale;
                    }
                    dofIndices++;
                }
            }
        }
    } else {
        for (int ibone = 0; ibone < deltaF->GetNumBones(); ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                *reinterpret_cast<COORD3 *>(&sqt[dofIndices[ibone]]) =
                    *reinterpret_cast<COORD3 *>(&mPrevValues[ibone]);
            }
        }
    }
    if (deltaF->mNumConstBones != 0) {
        unsigned short *constBoneIdxs = deltaF->GetConstBoneIdx();
        float *constPhys = deltaF->GetConstPhysical();
        unsigned short dofIndex;
        unsigned short boneIndex;
        int ibone;

        for (ibone = 0; ibone < deltaF->mNumConstBones; ibone++) {
            dofIndex = *constBoneIdxs++;
            boneIndex = dofIndex / 0xC;
            if (boneMask->GetBone(boneIndex)) {
                sqt[dofIndex++] = *constPhys++;
                sqt[dofIndex++] = *constPhys++;
                sqt[dofIndex++] = *constPhys++;
            } else {
                constPhys += 3;
            }
        }
    }

    return true;
}

void FnDeltaF3::InitBuffersAsRequired() {
    DeltaF3 *deltaF = reinterpret_cast<DeltaF3 *>(mpAnim);

    if (deltaF->GetNumBones() != 0) {
        mPrevValues = reinterpret_cast<COORD4 *>(
            MemoryPoolManager::NewBlock(deltaF->GetNumBones() * sizeof(*mPrevValues)));
        mPrevVBlock = mPrevValues;

        mNextValues = reinterpret_cast<COORD4 *>(
            MemoryPoolManager::NewBlock(deltaF->GetNumBones() * sizeof(*mNextValues)));
        mNextVBlock = mNextValues;

        mMinRangesf = reinterpret_cast<DeltaF3MinRange *>(
            MemoryPoolManager::NewBlock(deltaF->GetNumBones() * sizeof(*mMinRangesf)));

        DeltaF3::DofInfo *dofInfo = deltaF->GetDofInfo();

        for (int b = 0; b < deltaF->GetNumBones(); b++) {
            for (int c = 0; c < 3; c++) {
                mMinRangesf[b].mPhysMin[c] = dofInfo->mPhysMin[c];

                mMinRangesf[b].mPhysRange[c] = dofInfo->mPhysRange[c];
                mMinRangesf[b].mPhysRange[c] *= 1.5259022e-5f;

                mMinRangesf[b].mDeltaMin[c] =
                    2 * (dofInfo->mQuantMin[c] * 1.5259022e-5f) * dofInfo->mPhysRange[c] -
                    dofInfo->mPhysRange[c];

                mMinRangesf[b].mDeltaRange[c] =
                    2 * (dofInfo->mQuantRange[c] * 1.5259022e-5f) * dofInfo->mPhysRange[c];
                mMinRangesf[b].mDeltaRange[c] *= 0.003921569f;
            }
            dofInfo++;
        }
    }
}

} // namespace EAGLAnim
