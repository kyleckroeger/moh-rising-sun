// AI-assisted reconstruction; see docs/Animation.md and NOTICE.
#pragma implementation "FnDeltaSingleQ.h"
#include "EAGLAnim/FnDeltaSingleQ.h"
namespace EAGLAnim {
static inline void SingleQPostX(const COORD4 &q, const COORD4 &post, COORD4 &out) {
    out.x = q.x * post.w + q.w * post.x;
    out.y = q.x * post.z + q.w * post.y;
    out.z = -q.x * post.y + q.w * post.z;
    out.w = -q.x * post.x + q.w * post.w;
}
static inline void SingleQPreY(const COORD4 &pre, const COORD4 &q, COORD4 &t) {
    t.x = pre.x * q.w;
    t.y = pre.w * q.y;
    t.z = -pre.x * q.y;
    t.w = pre.w * q.w;
}
static inline void SingleQPostZ(const COORD4 &t, const COORD4 &post, COORD4 &out) {
    out.x = t.x * post.w - t.y * post.z;
    out.y = t.x * post.z + t.y * post.w;
    out.z = t.z * post.w + t.w * post.z;
    out.w = -t.z * post.z + t.w * post.w;
}
static inline void SingleQPreZ(const COORD4 &pre, const COORD4 &q, COORD4 &out) {
    out.x = pre.x * q.w - pre.y * q.z;
    out.y = pre.x * q.z + pre.y * q.w;
    out.z = pre.z * q.w + pre.w * q.z;
    out.w = -pre.z * q.z + pre.w * q.w;
}
static inline void SingleQRestoreY(const COORD4 &pre, const COORD4 &q, const COORD4 &post, COORD4 &out) {
    COORD4 t;
    SingleQPreY(pre, q, t);
    SingleQPostZ(t, post, out);
}
bool FnDeltaSingleQ::EvalSQT(float currTime, float *sqt, const BoneMask *boneMask) {
    if (boneMask)
        return EvalSQTMasked(currTime, boneMask, sqt);
    InitBuffersAsRequired();
    DeltaSingleQ *deltaQ = reinterpret_cast<DeltaSingleQ *>(mpAnim);
    int floorTime = FloatToInt(currTime);
    int floorKey;
    if (!deltaQ->mTimes) {
        if (floorTime < 0) {
            floorKey = 0;
        } else {
            if (floorTime >= deltaQ->mNumKeys) {
                floorKey = deltaQ->mNumKeys - 1;
            } else {
                floorKey = floorTime;
            }
        }
    } else if (floorTime < deltaQ->mTimes[0]) {
        floorKey = 0;
    } else {
        int timeIndex;
        if (mPrevKey < 1) {
            timeIndex = 0;
        } else {
            timeIndex = mPrevKey - 1;
        }
        if (deltaQ->mTimes[timeIndex] <= floorTime) {
            while (timeIndex < deltaQ->mNumKeys - 2 && deltaQ->mTimes[timeIndex + 1] <= floorTime) {
                timeIndex++;
            }
        } else {
            while (timeIndex > 0 && deltaQ->mTimes[timeIndex] > floorTime) {
                timeIndex--;
            }
        }

        floorKey = timeIndex + 1;
    }

    unsigned int binLenPower = deltaQ->GetBinLengthPower();
    int floorBinIdx = floorKey >> binLenPower;
    unsigned char *binData = mBins + floorBinIdx * mBinSize;
    DeltaSingleQPhysical *floorPhys = deltaQ->GetPhysical(binData);
    unsigned int binLenModMask = deltaQ->GetBinLengthModMask();
    int floorDeltaIdx = floorKey & binLenModMask;
    int prevBinIdx = mPrevKey >> binLenPower;
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
    int prevDeltaIdx;
    if (mPrevKey == -1 || floorBinIdx != prevBinIdx || floorKey < mPrevKey) {
        for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
            floorPhys[ibone].UnQuantize(mMinRanges[ibone].mIndex, mPrevQs[ibone]);
        }
        prevDeltaIdx = 0;
    } else
        prevDeltaIdx = mPrevKey & binLenModMask;
    int ceilKey = floorKey + 1;
    if (prevDeltaIdx < floorDeltaIdx) {
        DeltaSingleQDelta *d = reinterpret_cast<DeltaSingleQDelta *>(floorPhys + deltaQ->mNumBones) +
                               prevDeltaIdx * deltaQ->mNumBones;
        COORD4 delta;
        DeltaSingleQMinRangef ranges;
        for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                mMinRanges[ibone].UnQuantize(ranges);
                d->UnQuantize(ranges, delta);
                mPrevQs[ibone].x += delta.x;
                mPrevQs[ibone].y += delta.y;
                mPrevQs[ibone].z += delta.z;
                mPrevQs[ibone].w += delta.w;
                d++;
            }
        }
    }
    mPrevKey = floorKey;
    float scale;
    bool lerpReqd;
    if (!deltaQ->mTimes) {
        lerpReqd = currTime != floorTime;
        if (lerpReqd) {
            scale = currTime - floorTime;
        }
    } else if (floorKey == 0) {
        lerpReqd = currTime != 0.0f;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
            scale = currTime / ceilKeyTime;
        }
    } else {
        float floorKeyTime = deltaQ->mTimes[floorKey - 1];
        lerpReqd = currTime != floorKeyTime;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
            scale = (currTime - floorKeyTime) / (ceilKeyTime - floorKeyTime);
        }
    }
    if (lerpReqd && floorKey < deltaQ->mNumKeys - 1) {
        int ceilBinIdx = ceilKey >> binLenPower;
        DeltaSingleQPhysical *phys = reinterpret_cast<DeltaSingleQPhysical *>(mBins + ceilBinIdx * mBinSize);
        COORD4 q;
        COORD4 result;
        if (ceilBinIdx != floorBinIdx) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                phys[ibone].UnQuantize(mMinRanges[ibone].mIndex, q);
                LerpNormalizedQ(mPrevQs[ibone], q, scale, result);
                switch (mMinRanges[ibone].mIndex) {
                case 0:
                    SingleQPostX(result, mPostMultQs[ibone],
                                 *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 1:
                    SingleQRestoreY(mPreMultQs[ibone], result, mPostMultQs[ibone],
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 2:
                default:
                    SingleQPreZ(mPreMultQs[ibone], result,
                                *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                }
            }
        } else {
            DeltaSingleQDelta *d = reinterpret_cast<DeltaSingleQDelta *>(phys + deltaQ->mNumBones) +
                                   floorDeltaIdx * deltaQ->mNumBones;
            DeltaSingleQMinRangef ranges;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                mMinRanges[ibone].UnQuantize(ranges);
                d->UnQuantize(ranges, q);
                q.x += mPrevQs[ibone].x;
                q.y += mPrevQs[ibone].y;
                q.z += mPrevQs[ibone].z;
                q.w += mPrevQs[ibone].w;
                LerpNormalizedQ(mPrevQs[ibone], q, scale, result);
                switch (mMinRanges[ibone].mIndex) {
                case 0:
                    SingleQPostX(result, mPostMultQs[ibone],
                                 *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 1:
                    SingleQRestoreY(mPreMultQs[ibone], result, mPostMultQs[ibone],
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 2:
                default:
                    SingleQPreZ(mPreMultQs[ibone], result,
                                *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                }

                d++;
            }
        }
    } else {
        for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
            {
                switch (mMinRanges[ibone].mIndex) {
                case 0:
                    SingleQPostX(mPrevQs[ibone], mPostMultQs[ibone],
                                 *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 1:
                    SingleQRestoreY(mPreMultQs[ibone], mPrevQs[ibone], mPostMultQs[ibone],
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 2:
                default:
                    SingleQPreZ(mPreMultQs[ibone], mPrevQs[ibone],
                                *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                }
            }
        }
    }

    return true;
}
bool FnDeltaSingleQ::EvalSQTMasked(float currTime, const BoneMask *boneMask, float *sqt) {
    InitBuffersAsRequired();
    DeltaSingleQ *deltaQ = reinterpret_cast<DeltaSingleQ *>(mpAnim);
    int floorTime = FloatToInt(currTime);
    int floorKey;
    if (!deltaQ->mTimes) {
        if (floorTime < 0) {
            floorKey = 0;
        } else {
            if (floorTime >= deltaQ->mNumKeys) {
                floorKey = deltaQ->mNumKeys - 1;
            } else {
                floorKey = floorTime;
            }
        }
    } else if (floorTime < deltaQ->mTimes[0]) {
        floorKey = 0;
    } else {
        int timeIndex;
        if (mPrevKey < 1) {
            timeIndex = 0;
        } else {
            timeIndex = mPrevKey - 1;
        }
        if (deltaQ->mTimes[timeIndex] <= floorTime) {
            while (timeIndex < deltaQ->mNumKeys - 2 && deltaQ->mTimes[timeIndex + 1] <= floorTime) {
                timeIndex++;
            }
        } else {
            while (timeIndex > 0 && deltaQ->mTimes[timeIndex] > floorTime) {
                timeIndex--;
            }
        }

        floorKey = timeIndex + 1;
    }

    unsigned int binLenPower = deltaQ->GetBinLengthPower();
    int floorBinIdx = floorKey >> binLenPower;
    unsigned char *binData = mBins + floorBinIdx * mBinSize;
    DeltaSingleQPhysical *floorPhys = deltaQ->GetPhysical(binData);
    unsigned int binLenModMask = deltaQ->GetBinLengthModMask();
    int floorDeltaIdx = floorKey & binLenModMask;
    int prevBinIdx = mPrevKey >> binLenPower;
    unsigned char *boneIdxs = deltaQ->mBoneIdxs;
    int prevDeltaIdx;
    if (mPrevKey == -1 || floorBinIdx != prevBinIdx || floorKey < mPrevKey) {
        for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone]))
                floorPhys[ibone].UnQuantize(mMinRanges[ibone].mIndex, mPrevQs[ibone]);
        }
        prevDeltaIdx = 0;
    } else
        prevDeltaIdx = mPrevKey & binLenModMask;
    int ceilKey = floorKey + 1;
    if (prevDeltaIdx < floorDeltaIdx) {
        DeltaSingleQDelta *d = reinterpret_cast<DeltaSingleQDelta *>(floorPhys + deltaQ->mNumBones) +
                               prevDeltaIdx * deltaQ->mNumBones;
        COORD4 delta;
        DeltaSingleQMinRangef ranges;
        for (int iframe = prevDeltaIdx; iframe < floorDeltaIdx; iframe++) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    mMinRanges[ibone].UnQuantize(ranges);
                    d->UnQuantize(ranges, delta);
                    mPrevQs[ibone].x += delta.x;
                    mPrevQs[ibone].y += delta.y;
                    mPrevQs[ibone].z += delta.z;
                    mPrevQs[ibone].w += delta.w;
                }
                d++;
            }
        }
    }
    mPrevKey = floorKey;
    float scale;
    bool lerpReqd;
    if (!deltaQ->mTimes) {
        lerpReqd = currTime != floorTime;
        if (lerpReqd) {
            scale = currTime - floorTime;
        }
    } else if (floorKey == 0) {
        lerpReqd = currTime != 0.0f;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
            scale = currTime / ceilKeyTime;
        }
    } else {
        float floorKeyTime = deltaQ->mTimes[floorKey - 1];
        lerpReqd = currTime != floorKeyTime;
        if (lerpReqd) {
            float ceilKeyTime = static_cast<float>(deltaQ->mTimes[floorKey]);
            scale = (currTime - floorKeyTime) / (ceilKeyTime - floorKeyTime);
        }
    }
    if (lerpReqd && floorKey < deltaQ->mNumKeys - 1) {
        int ceilBinIdx = ceilKey >> binLenPower;
        DeltaSingleQPhysical *phys = reinterpret_cast<DeltaSingleQPhysical *>(mBins + ceilBinIdx * mBinSize);
        COORD4 q;
        COORD4 result;
        if (ceilBinIdx != floorBinIdx) {
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    phys[ibone].UnQuantize(mMinRanges[ibone].mIndex, q);
                    LerpNormalizedQ(mPrevQs[ibone], q, scale, result);
                    switch (mMinRanges[ibone].mIndex) {
                    case 0:
                        SingleQPostX(result, mPostMultQs[ibone],
                                     *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                        break;
                    case 1:
                        SingleQRestoreY(mPreMultQs[ibone], result, mPostMultQs[ibone],
                                        *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                        break;
                    case 2:
                    default:
                        SingleQPreZ(mPreMultQs[ibone], result,
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                        break;
                    }
                }
            }
        } else {
            DeltaSingleQDelta *d = reinterpret_cast<DeltaSingleQDelta *>(phys + deltaQ->mNumBones) +
                                   floorDeltaIdx * deltaQ->mNumBones;
            DeltaSingleQMinRangef ranges;
            for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
                if (boneMask->GetBone(boneIdxs[ibone])) {
                    mMinRanges[ibone].UnQuantize(ranges);
                    d->UnQuantize(ranges, q);
                    q.x += mPrevQs[ibone].x;
                    q.y += mPrevQs[ibone].y;
                    q.z += mPrevQs[ibone].z;
                    q.w += mPrevQs[ibone].w;
                    LerpNormalizedQ(mPrevQs[ibone], q, scale, result);
                    switch (mMinRanges[ibone].mIndex) {
                    case 0:
                        SingleQPostX(result, mPostMultQs[ibone],
                                     *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                        break;
                    case 1:
                        SingleQRestoreY(mPreMultQs[ibone], result, mPostMultQs[ibone],
                                        *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                        break;
                    case 2:
                    default:
                        SingleQPreZ(mPreMultQs[ibone], result,
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                        break;
                    }
                }
                d++;
            }
        }
    } else {
        for (int ibone = 0; ibone < deltaQ->mNumBones; ibone++) {
            if (boneMask->GetBone(boneIdxs[ibone])) {
                switch (mMinRanges[ibone].mIndex) {
                case 0:
                    SingleQPostX(mPrevQs[ibone], mPostMultQs[ibone],
                                 *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 1:
                    SingleQRestoreY(mPreMultQs[ibone], mPrevQs[ibone], mPostMultQs[ibone],
                                    *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                case 2:
                default:
                    SingleQPreZ(mPreMultQs[ibone], mPrevQs[ibone],
                                *reinterpret_cast<COORD4 *>(&sqt[boneIdxs[ibone] * 12 + 4]));
                    break;
                }
            }
        }
    }

    return true;
}
} // namespace EAGLAnim
