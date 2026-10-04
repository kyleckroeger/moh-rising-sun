// Adapted from the CC0 NFS Most Wanted reconstruction; see docs/Animation.md.
#include "EAGLAnim/DeltaChan.h"
#include "EAGLTransform.h"
#include <math.h>
namespace EAGLAnim {
inline void FastQuatBlendF4(float w, const float *d0, const float *d1, float *out) {
    if (d0[0] * d1[0] + d0[1] * d1[1] + d0[2] * d1[2] + d0[3] * d1[3] > 0.0f) {
        out[0] = w * (d1[0] - d0[0]) + d0[0];
        out[1] = w * (d1[1] - d0[1]) + d0[1];
        out[2] = w * (d1[2] - d0[2]) + d0[2];
        out[3] = w * (d1[3] - d0[3]) + d0[3];
    } else {
        out[0] = d0[0] - w * (d1[0] + d0[0]);
        out[1] = d0[1] - w * (d1[1] + d0[1]);
        out[2] = d0[2] - w * (d1[2] + d0[2]);
        out[3] = d0[3] - w * (d1[3] + d0[3]);
    }

    float s = 1.0f / sqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2] + out[3] * out[3]);

    out[0] *= s;
    out[1] *= s;
    out[2] *= s;
    out[3] *= s;
}

inline void FnDeltaChan::EvalToPrevValues(int frame) {
    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
    DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
    const int numFrames = deltaChan->GetNumFrames();

    if (frame >= numFrames) {
        frame = numFrames - 1;
    } else if (frame < 0) {
        frame = 0;
    }

    unsigned short numDofs = deltaData->GetNumDofs();
    deltaData->DecompressValues(0, numDofs, mPrevFrame, frame, mPrevValues, mPrevValues);
    mPrevFrame = frame;
}

inline void FnDeltaChan::EvalToPrevValues(int frame, int numDofPerBone, int, const unsigned short *) {
    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
    DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
    const int numFrames = deltaChan->GetNumFrames();

    if (frame >= numFrames) {
        frame = numFrames - 1;
    } else if (frame < 0) {
        frame = 0;
    }

    deltaData->DecompressValues(numDofPerBone, mPrevFrame, frame, mPrevValues, mPrevValues, mNumDofs,
                                mDofMask);
    mPrevFrame = frame;
}

inline void FnKeyDeltaChan::EvalToPrevValues(int key) {
    KeyDeltaChan *keyChan = reinterpret_cast<KeyDeltaChan *>(mpAnim);
    DeltaCompressedData *deltaData = keyChan->GetDeltaData();
    unsigned short numDofs = keyChan->GetNumDofs();

    deltaData->DecompressValues(0, numDofs, mPrevKey, key, mPrevValues, mPrevValues);
    mPrevKey = key;
}

inline void FnKeyDeltaChan::EvalToPrevValues(int key, int numDofPerBone, int, const unsigned short *) {
    KeyDeltaChan *keyChan = reinterpret_cast<KeyDeltaChan *>(mpAnim);
    DeltaCompressedData *deltaData = keyChan->GetDeltaData();

    deltaData->DecompressValues(numDofPerBone, mPrevKey, key, mPrevValues, mPrevValues, mNumDofs, mDofMask);
    mPrevKey = key;
}

inline int FnKeyDeltaChan::FindLowerKey(float currTime) {
    int floorTime = FloatToInt(currTime);
    KeyDeltaChan *keyChan = reinterpret_cast<KeyDeltaChan *>(mpAnim);
    int numKeys = keyChan->GetNumKeys();
    unsigned short *keyTimes = keyChan->GetKeyTimes();

    if (currTime < keyTimes[0]) {
        return 0;
    }

    int lowerKey;
    if (mPrevKey <= 0) {
        lowerKey = 0;
    } else {
        lowerKey = mPrevKey - 1;
    }

    if (keyTimes[lowerKey] <= floorTime) {
        while (lowerKey < numKeys - 2 && keyTimes[lowerKey + 1] <= floorTime) {
            lowerKey++;
        }
    } else {
        while (lowerKey > 0 && keyTimes[lowerKey] > floorTime) {
            lowerKey--;
        }
    }

    return lowerKey + 1;
}

void FnDeltaChan::SetAnimMemoryMap(AnimMemoryMap *anim) {
    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(anim);
    if (mPrevValues) {
        if (deltaChan->GetNumDofs() >= reinterpret_cast<DeltaChan *>(mpAnim)->GetNumDofs()) {
            MemoryPoolManager::DeleteBlock(mPrevValues);
            mPrevValues = reinterpret_cast<float *>(
                MemoryPoolManager::NewBlock(deltaChan->GetNumDofs() * sizeof(*mPrevValues)));
        }
    } else {
        mPrevValues = reinterpret_cast<float *>(
            MemoryPoolManager::NewBlock(deltaChan->GetNumDofs() * sizeof(*mPrevValues)));
    }
    mpAnim = deltaChan;
    mPrevFrame = -1;
}

void FnDeltaLerpChan::Eval(float prevTime, float currTime, float *evalBuffer) {
    int floorFrame = FloatToInt(currTime);
    int ceilFrame = floorFrame + 1;
    EvalToPrevValues(floorFrame);

    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
    int numFrames = deltaChan->GetNumFrames();
    DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
    unsigned short numDofs = deltaData->GetNumDofs();
    unsigned short *dofIndices = deltaChan->GetDofIndices();

    if (currTime == static_cast<float>(floorFrame) || ceilFrame >= numFrames) {
        for (int idof = 0; idof < numDofs; idof++) {
            evalBuffer[dofIndices[idof]] = mPrevValues[idof];
        }
    } else {
        float t = currTime - static_cast<float>(floorFrame);
        deltaData->DecompressValuesIndexed(0, numDofs, floorFrame, ceilFrame, mPrevValues, evalBuffer, 1,
                                           dofIndices, t);
    }
}

bool FnDeltaLerpChan::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (!boneMask) {
        if (mBoneMask) {
            mPrevFrame = -1;
            mBoneMask = 0;
        }
        int floorFrame = FloatToInt(currTime);
        int ceilFrame = floorFrame + 1;
        EvalToPrevValues(floorFrame);

        DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
        int numFrames = deltaChan->GetNumFrames();
        DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
        unsigned short numDofs = deltaData->GetNumDofs();
        unsigned short *dofIndices = deltaChan->GetDofIndices();

        if (currTime == static_cast<float>(floorFrame) || ceilFrame >= numFrames) {
            for (int idof = 0; idof < numDofs; idof++) {
                sqt[dofIndices[idof]] = mPrevValues[idof];
            }
        } else {
            float t = currTime - static_cast<float>(floorFrame);
            deltaData->DecompressValuesIndexed(0, numDofs, floorFrame, ceilFrame, mPrevValues, sqt, 1,
                                               dofIndices, t);
        }
    } else {
        return EvalSQTMask(currTime, sqt, boneMask);
    }
    return true;
}

bool FnDeltaLerpChan::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mBoneMask = boneMask;
        mPrevFrame = -1;
    }

    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
    int numFrames = deltaChan->GetNumFrames();
    DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
    unsigned short numDofs = deltaData->GetNumDofs();
    unsigned short *dofIndices = deltaChan->GetDofIndices();

    int idof;
    int boneIdx;
    if (mDofMask == 0 && numDofs != 0) {
        mDofMask =
            reinterpret_cast<unsigned short *>(MemoryPoolManager::NewBlock(numDofs * sizeof(*mDofMask)));
        for (idof = 0; idof < numDofs; idof++) {
            boneIdx = (dofIndices[idof] / 0xCu) & 0xFF;
            if (boneMask->GetBone(boneIdx)) {
                mDofMask[mNumDofs++] = idof;
            }
        }
    }

    int floorFrame = FloatToInt(currTime);
    int ceilFrame = floorFrame + 1;

    EvalToPrevValues(floorFrame, 1, mNumDofs, mDofMask);

    if (currTime == static_cast<float>(floorFrame) || ceilFrame >= numFrames) {
        for (idof = 0; idof < mNumDofs; idof++) {
            sqt[dofIndices[mDofMask[idof]]] = mPrevValues[mDofMask[idof]];
        }
    } else {
        float t = currTime - static_cast<float>(floorFrame);
        deltaData->DecompressValuesIndexed(floorFrame, ceilFrame, mPrevValues, sqt, 1, dofIndices, t,
                                           mNumDofs, mDofMask);
    }
    return true;
}

void FnDeltaQuatChan::Eval(float prevTime, float currTime, float *evalBuffer) {
    int floorFrame = FloatToInt(currTime);
    int ceilFrame = floorFrame + 1;
    EvalToPrevValues(floorFrame);

    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
    int numFrames = deltaChan->GetNumFrames();
    DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
    unsigned short numDofs = deltaData->GetNumDofs();
    unsigned short *dofIndices = deltaChan->GetDofIndices();

    int idof;
    int iquat;

    if (currTime == static_cast<float>(floorFrame) || ceilFrame >= numFrames) {
        for (iquat = 0, idof = 0; iquat < numDofs; iquat += 4, idof++) {
            *reinterpret_cast<COORD4 *>(&evalBuffer[dofIndices[idof]]) =
                *reinterpret_cast<COORD4 *>(&mPrevValues[iquat]);
        }
    } else {
        float t = currTime - static_cast<float>(floorFrame);
        deltaData->DecompressValuesIndexed(0, numDofs, floorFrame, ceilFrame, mPrevValues, evalBuffer, 4,
                                           dofIndices, t);
    }
}

bool FnDeltaQuatChan::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (!boneMask) {
        if (mBoneMask) {
            mPrevFrame = -1;
            mBoneMask = 0;
        }

        DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
        int numFrames = deltaChan->GetNumFrames();
        DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
        unsigned short numDofs = deltaData->GetNumDofs();
        unsigned short *dofIndices = deltaChan->GetDofIndices();

        int idof;
        int iquat;

        int floorFrame = FloatToInt(currTime);
        int ceilFrame = floorFrame + 1;
        EvalToPrevValues(floorFrame);

        if (currTime == static_cast<float>(floorFrame) || ceilFrame >= numFrames) {
            for (iquat = 0, idof = 0; iquat < numDofs; iquat += 4, idof++) {
                *reinterpret_cast<COORD4 *>(&sqt[dofIndices[idof]]) =
                    *reinterpret_cast<COORD4 *>(&mPrevValues[iquat]);
            }
        } else {
            float t = currTime - static_cast<float>(floorFrame);
            deltaData->DecompressValuesIndexed(0, numDofs, floorFrame, ceilFrame, mPrevValues, sqt, 4,
                                               dofIndices, t);
        }
    } else {
        return EvalSQTMask(currTime, sqt, boneMask);
    }
    return true;
}

bool FnDeltaQuatChan::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mBoneMask = boneMask;
        mPrevFrame = -1;
    }

    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(mpAnim);
    int numFrames = deltaChan->GetNumFrames();
    DeltaCompressedData *deltaData = deltaChan->GetDeltaData();
    unsigned short numDofs = deltaData->GetNumDofs();
    unsigned short *dofIndices = deltaChan->GetDofIndices();

    int idof;
    int iquat;

    int boneIdx;
    if (mDofMask == 0 && numDofs != 0) {
        mDofMask =
            reinterpret_cast<unsigned short *>(MemoryPoolManager::NewBlock(numDofs * sizeof(*mDofMask)));
        for (iquat = 0, idof = 0; iquat < numDofs; iquat += 4, idof++) {
            boneIdx = (dofIndices[idof] / 0xCu) & 0xFF;
            if (boneMask->GetBone(boneIdx)) {
                mDofMask[mNumDofs++] = idof;
            }
        }
    }

    int floorFrame = FloatToInt(currTime);
    int ceilFrame = floorFrame + 1;
    EvalToPrevValues(floorFrame, 4, mNumDofs, mDofMask);

    if (currTime == static_cast<float>(floorFrame) || ceilFrame >= numFrames) {
        for (idof = 0; idof < mNumDofs; idof++) {
            *reinterpret_cast<COORD4 *>(&sqt[dofIndices[mDofMask[idof]]]) =
                *reinterpret_cast<COORD4 *>(&mPrevValues[mDofMask[idof] * 4]);
        }
    } else {
        float t = currTime - static_cast<float>(floorFrame);
        deltaData->DecompressValuesIndexed(floorFrame, ceilFrame, mPrevValues, sqt, 4, dofIndices, t,
                                           mNumDofs, mDofMask);
    }
    return true;
}

void FnKeyDeltaChan::SetAnimMemoryMap(AnimMemoryMap *anim) {
    DeltaChan *deltaChan = reinterpret_cast<DeltaChan *>(anim);
    if (mPrevValues) {
        if (deltaChan->GetNumDofs() >= reinterpret_cast<DeltaChan *>(mpAnim)->GetNumDofs()) {
            MemoryPoolManager::DeleteBlock(mPrevValues);
            mPrevValues = reinterpret_cast<float *>(
                MemoryPoolManager::NewBlock(deltaChan->GetNumDofs() * sizeof(*mPrevValues)));
        }
    } else {
        mPrevValues = reinterpret_cast<float *>(
            MemoryPoolManager::NewBlock(deltaChan->GetNumDofs() * sizeof(*mPrevValues)));
    }
    mpAnim = deltaChan;
    mPrevKey = -1;
}

bool FnKeyLerpChan::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (!boneMask) {
        if (mBoneMask) {
            mPrevKey = -1;
            mBoneMask = 0;
        }
        int lowKey = FindLowerKey(currTime);
        EvalToPrevValues(lowKey);

        KeyLerpChan *keyChan = reinterpret_cast<KeyLerpChan *>(mpAnim);
        int numKeys = keyChan->GetNumKeys();
        DeltaCompressedData *deltaData = keyChan->GetDeltaData();
        unsigned short numDofs = deltaData->GetNumDofs();
        unsigned short *keyTimes = keyChan->GetKeyTimes();
        unsigned short *dofIndices = keyChan->GetDofIndices();

        int lowKeyTime;
        // The key-time index -1 denotes the implicit initial sample at time zero.
        if (lowKey - 1 == -1) {
            lowKeyTime = 0;
        } else {
            lowKeyTime = keyTimes[lowKey - 1];
        };

        if (lowKeyTime == currTime || (lowKey == numKeys - 1 && currTime > keyTimes[numKeys - 2]) ||
            (lowKey == 0 && currTime < 0.0f)) {
            for (int idof = 0; idof < numDofs; idof++) {
                sqt[dofIndices[idof]] = mPrevValues[idof];
            }
        } else {
            float t = (currTime - lowKeyTime) / (keyTimes[lowKey] - lowKeyTime);
            deltaData->DecompressValuesIndexed(0, numDofs, lowKey, lowKey + 1, mPrevValues, sqt, 1,
                                               dofIndices, t);
        }
    } else {
        return EvalSQTMask(currTime, sqt, boneMask);
    }
    return true;
}

bool FnKeyLerpChan::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mBoneMask = boneMask;
        mPrevKey = -1;
    }

    KeyLerpChan *keyChan = reinterpret_cast<KeyLerpChan *>(mpAnim);
    int numKeys = keyChan->GetNumKeys();
    DeltaCompressedData *deltaData = keyChan->GetDeltaData();
    unsigned short numDofs = deltaData->GetNumDofs();
    unsigned short *keyTimes = keyChan->GetKeyTimes();
    unsigned short *dofIndices = keyChan->GetDofIndices();

    int idof;
    int boneIdx;
    if (mDofMask == 0 && numDofs != 0) {
        mDofMask =
            reinterpret_cast<unsigned short *>(MemoryPoolManager::NewBlock(numDofs * sizeof(*mDofMask)));
        for (idof = 0; idof < numDofs; idof++) {
            boneIdx = (dofIndices[idof] / 0xCu) & 0xFF;
            if (boneMask->GetBone(boneIdx)) {
                mDofMask[mNumDofs++] = idof;
            }
        }
    }

    int lowKey = FindLowerKey(currTime);
    EvalToPrevValues(lowKey, 1, mNumDofs, mDofMask);
    int lowKeyTime;
    // The key-time index -1 denotes the implicit initial sample at time zero.
    if (lowKey - 1 == -1) {
        lowKeyTime = 0;
    } else {
        lowKeyTime = keyTimes[lowKey - 1];
    };

    if (lowKeyTime == currTime || (lowKey == numKeys - 1 && currTime > keyTimes[numKeys - 2]) ||
        (lowKey == 0 && currTime < 0.0f)) {
        for (idof = 0; idof < mNumDofs; idof++) {
            sqt[dofIndices[mDofMask[idof]]] = mPrevValues[mDofMask[idof]];
        }
    } else {
        float t = (currTime - lowKeyTime) / (keyTimes[lowKey] - lowKeyTime);
        deltaData->DecompressValuesIndexed(lowKey, lowKey + 1, mPrevValues, sqt, 1, dofIndices, t, mNumDofs,
                                           mDofMask);
    }
    return true;
}

bool FnKeyQuatChan::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (!boneMask) {
        if (mBoneMask) {
            mPrevKey = -1;
            mBoneMask = 0;
        }

        KeyQuatChan *keyChan = reinterpret_cast<KeyQuatChan *>(mpAnim);
        int numKeys = keyChan->GetNumKeys();
        DeltaCompressedData *deltaData = keyChan->GetDeltaData();
        unsigned short numDofs = deltaData->GetNumDofs();
        unsigned short *keyTimes = keyChan->GetKeyTimes();
        unsigned short *dofIndices = keyChan->GetDofIndices();

        int lowKey = FindLowerKey(currTime);
        EvalToPrevValues(lowKey);

        int iquat;
        int idof;

        int lowKeyTime;
        // The key-time index -1 denotes the implicit initial sample at time zero.
        if (lowKey - 1 == -1) {
            lowKeyTime = 0;
        } else {
            lowKeyTime = keyTimes[lowKey - 1];
        };

        if (lowKeyTime == currTime || (lowKey == numKeys - 1 && currTime > keyTimes[numKeys - 2]) ||
            (lowKey == 0 && currTime < 0.0f)) {
            for (iquat = 0, idof = 0; iquat < numDofs; iquat += 4, idof++) {
                *reinterpret_cast<COORD4 *>(&sqt[dofIndices[idof]]) =
                    *reinterpret_cast<COORD4 *>(&mPrevValues[iquat]);
            }
        } else {
            float t = (currTime - lowKeyTime) / (keyTimes[lowKey] - lowKeyTime);
            COORD4 ceilQuat;
            for (iquat = 0, idof = 0; iquat < numDofs; iquat += 4, idof++) {
                deltaData->DecompressValues(iquat, 4, lowKey, lowKey + 1, &mPrevValues[iquat],
                                            reinterpret_cast<float *>(&ceilQuat));

                float *blendValue = &sqt[dofIndices[idof]];
                FastQuatBlendF4(t, &mPrevValues[iquat], reinterpret_cast<float *>(&ceilQuat), blendValue);
            }
        }
    } else {
        return EvalSQTMask(currTime, sqt, boneMask);
    }
    return true;
}

bool FnKeyQuatChan::EvalSQTMask(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask != mBoneMask) {
        mBoneMask = boneMask;
        mPrevKey = -1;
    }

    KeyQuatChan *keyChan = reinterpret_cast<KeyQuatChan *>(mpAnim);
    int numKeys = keyChan->GetNumKeys();
    DeltaCompressedData *deltaData = keyChan->GetDeltaData();
    unsigned short numDofs = deltaData->GetNumDofs();
    unsigned short *keyTimes = keyChan->GetKeyTimes();
    unsigned short *dofIndices = keyChan->GetDofIndices();

    int idof;
    int iquat;
    int boneIdx;
    if (mDofMask == 0 && numDofs != 0) {
        mDofMask =
            reinterpret_cast<unsigned short *>(MemoryPoolManager::NewBlock(numDofs * sizeof(*mDofMask)));
        for (iquat = 0, idof = 0; iquat < numDofs; iquat += 4, idof++) {
            boneIdx = (dofIndices[idof] / 0xCu) & 0xFF;
            if (boneMask->GetBone(boneIdx)) {
                mDofMask[mNumDofs++] = idof;
            }
        }
    }

    int lowKey = FindLowerKey(currTime);
    EvalToPrevValues(lowKey, 4, mNumDofs, mDofMask);
    int lowKeyTime;
    // The key-time index -1 denotes the implicit initial sample at time zero.
    if (lowKey - 1 == -1) {
        lowKeyTime = 0;
    } else {
        lowKeyTime = keyTimes[lowKey - 1];
    };

    if (lowKeyTime == currTime || (lowKey == numKeys - 1 && currTime > keyTimes[numKeys - 2]) ||
        (lowKey == 0 && currTime < 0.0f)) {
        for (idof = 0; idof < mNumDofs; idof++) {
            *reinterpret_cast<COORD4 *>(&sqt[dofIndices[mDofMask[idof]]]) =
                *reinterpret_cast<COORD4 *>(&mPrevValues[mDofMask[idof] * 4]);
        }
    } else {
        float t = (currTime - lowKeyTime) / (keyTimes[lowKey] - lowKeyTime);
        COORD4 ceilQuat;
        for (idof = 0; idof < mNumDofs; idof++) {
            deltaData->DecompressValues(mDofMask[idof] * 4, 4, lowKey, lowKey + 1,
                                        &mPrevValues[mDofMask[idof] * 4],
                                        reinterpret_cast<float *>(&ceilQuat));

            float *blendValue = &sqt[dofIndices[mDofMask[idof]]];
            FastQuatBlendF4(t, &mPrevValues[mDofMask[idof] * 4], reinterpret_cast<float *>(&ceilQuat),
                            blendValue);
        }
    }
    return true;
}

FnDeltaChan::FnDeltaChan() : mPrevFrame(-1), mBoneMask(0), mPrevValues(0), mNumDofs(0), mDofMask(0) {}

bool FnDeltaChan::GetLength(float &timeLength) const {
    timeLength = static_cast<float>(reinterpret_cast<DeltaChan *>(mpAnim)->GetNumFrames());

    return true;
}

FnDeltaChan::~FnDeltaChan() {
    if (mPrevValues) {
        MemoryPoolManager::DeleteBlock(mPrevValues);
    }
    if (mDofMask) {
        MemoryPoolManager::DeleteBlock(mDofMask);
    }
}

bool FnDeltaLerpChan::EvalWeights(float currTime, float *weights) {
    Eval(currTime, currTime, weights);
    return true;
}

bool FnDeltaLerpChan::EvalVel2D(float currTime, float *vel) {
    Eval(currTime, currTime, vel);
    return true;
}

bool FnDeltaLerpChan::GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
    DeltaChan *chan = reinterpret_cast<DeltaChan *>(mpAnim);
    int numDofs = chan->GetNumDofs();
    for (int i = numDofs - 1; i >= 0; i--) {
        unsigned short boneIdx = chan->GetDofIndices()[i] / 12u;
        if (!boneMask->GetBone(boneIdx)) {
            if (numBones == 0) {
                minBone = boneIdx;
                maxBone = boneIdx;
            } else {
                if (boneIdx < minBone)
                    minBone = boneIdx;
                if (boneIdx > maxBone)
                    maxBone = boneIdx;
            }
            numBones++;
            boneMask->SetBone(boneIdx, true);
        }
    }
    return true;
}

bool FnDeltaQuatChan::GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
    DeltaChan *chan = reinterpret_cast<DeltaChan *>(mpAnim);
    int numDofs = chan->GetNumDofs() / 4;
    for (int i = numDofs - 1; i >= 0; i--) {
        unsigned short boneIdx = chan->GetDofIndices()[i] / 12u;
        if (!boneMask->GetBone(boneIdx)) {
            if (numBones == 0) {
                minBone = boneIdx;
                maxBone = boneIdx;
            } else {
                if (boneIdx < minBone)
                    minBone = boneIdx;
                if (boneIdx > maxBone)
                    maxBone = boneIdx;
            }
            numBones++;
            boneMask->SetBone(boneIdx, true);
        }
    }
    return true;
}

FnKeyDeltaChan::~FnKeyDeltaChan() {
    if (mPrevValues) {
        MemoryPoolManager::DeleteBlock(mPrevValues);
    }
    if (mDofMask) {
        MemoryPoolManager::DeleteBlock(mDofMask);
    }
}

bool FnKeyDeltaChan::GetLength(float &timeLength) const {
    KeyDeltaChan *keyChan = reinterpret_cast<KeyDeltaChan *>(mpAnim);
    int numKeys = keyChan->GetNumKeys();
    timeLength = static_cast<float>(keyChan->GetKeyTimes()[numKeys - 2] + 1);

    return true;
}

void FnKeyLerpChan::Eval(float prevTime, float currTime, float *evalBuffer) {
    EvalSQT(currTime, evalBuffer, 0);
}

bool FnKeyLerpChan::GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
    KeyDeltaChan *chan = reinterpret_cast<KeyDeltaChan *>(mpAnim);
    int numDofs = chan->GetNumDofs();
    for (int i = numDofs - 1; i >= 0; i--) {
        unsigned short boneIdx = chan->GetDofIndices()[i] / 12u;
        if (!boneMask->GetBone(boneIdx)) {
            if (numBones == 0) {
                minBone = boneIdx;
                maxBone = boneIdx;
            } else {
                if (boneIdx < minBone)
                    minBone = boneIdx;
                if (boneIdx > maxBone)
                    maxBone = boneIdx;
            }
            numBones++;
            boneMask->SetBone(boneIdx, true);
        }
    }
    return true;
}

void FnKeyQuatChan::Eval(float prevTime, float currTime, float *evalBuffer) {
    EvalSQT(currTime, evalBuffer, 0);
}

bool FnKeyQuatChan::GetAnimatedBonesAux(BoneMask *boneMask, int &numBones, int &minBone, int &maxBone) {
    KeyDeltaChan *chan = reinterpret_cast<KeyDeltaChan *>(mpAnim);
    int numDofs = chan->GetNumDofs() / 4;
    for (int i = numDofs - 1; i >= 0; i--) {
        unsigned short boneIdx = chan->GetDofIndices()[i] / 12u;
        if (!boneMask->GetBone(boneIdx)) {
            if (numBones == 0) {
                minBone = boneIdx;
                maxBone = boneIdx;
            } else {
                if (boneIdx < minBone)
                    minBone = boneIdx;
                if (boneIdx > maxBone)
                    maxBone = boneIdx;
            }
            numBones++;
            boneMask->SetBone(boneIdx, true);
        }
    }
    return true;
}
} // namespace EAGLAnim
