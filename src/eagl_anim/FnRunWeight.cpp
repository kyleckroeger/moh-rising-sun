// Reconstructed from the pinned GR8E69 target; reference names follow the CC0
// dbalatoni13/nfsmw EAGL4Anim headers. See docs/Animation.md.
#include "EAGLAnim/FnRunBlender.h"
namespace EAGLAnim {
void FnRunBlender::SetWeight(float w) {
    float freq;
    int idx = FloatToInt(w);
    if (idx < 0)
        idx = 0;
    if (idx >= mNumAnims - 1)
        idx = mNumAnims - 2;
    mWeight = w - idx;
    if (idx == mIdx) {
        freq = mFreq;
        mFreq = mWeight / mCycles[1] + (1.0f - mWeight) / mCycles[0];
        mOffset = freq / mFreq * (mPrevTime + mOffset) - mPrevTime;
    } else {
        if (idx == mIdx + 1) {
            MemoryPoolManager::DeleteFnAnim(mFnAnims[0]);
            mFnAnims[0] = mFnAnims[1];
            mFnAnims[1] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mAnims[idx + 1]));
        } else if (idx == mIdx - 1) {
            MemoryPoolManager::DeleteFnAnim(mFnAnims[1]);
            mFnAnims[1] = mFnAnims[0];
            mFnAnims[0] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mAnims[idx]));
        } else {
            if (mFnAnims[0])
                MemoryPoolManager::DeleteFnAnim(mFnAnims[0]);
            if (mFnAnims[1])
                MemoryPoolManager::DeleteFnAnim(mFnAnims[1]);
            mFnAnims[0] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mAnims[idx]));
            mFnAnims[1] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mAnims[idx + 1]));
        }
        if (mVels) {
            if (idx == mIdx + 1) {
                MemoryPoolManager::DeleteFnAnim(mFnVelAnims[0]);
                mFnVelAnims[0] = mFnVelAnims[1];
                mFnVelAnims[1] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mVels[idx + 1]));
            } else if (idx == mIdx - 1) {
                MemoryPoolManager::DeleteFnAnim(mFnVelAnims[1]);
                mFnVelAnims[1] = mFnVelAnims[0];
                mFnVelAnims[0] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mVels[idx]));
            } else {
                if (mFnVelAnims[0])
                    MemoryPoolManager::DeleteFnAnim(mFnVelAnims[0]);
                if (mFnVelAnims[1])
                    MemoryPoolManager::DeleteFnAnim(mFnVelAnims[1]);
                mFnVelAnims[0] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mVels[idx]));
                mFnVelAnims[1] = MemoryPoolManager::NewFnAnim(const_cast<AnimMemoryMap *>(mVels[idx + 1]));
            }
        }
        if (mPhases[idx]->StartWithRight())
            mAlignFrame[0] = mPhases[idx]->mStartTime;
        else
            mAlignFrame[0] = mPhases[idx]->mStartTime + mPhases[idx]->mCycles[0];
        mAlignFrame[1] = mPhases[idx + 1]->mStartTime;
        if (!mPhases[idx + 1]->StartWithRight())
            mAlignFrame[1] += mPhases[idx + 1]->mCycles[0];
        mIdx = idx;
        mCycles[0] = (mPhases[mIdx]->mCycles[0] + mPhases[mIdx]->mCycles[1]) * 0.5f;
        mCycles[1] = (mPhases[mIdx + 1]->mCycles[0] + mPhases[mIdx + 1]->mCycles[1]) * 0.5f;
        freq = mFreq;
        mFreq = mWeight / mCycles[1] + (1.0f - mWeight) / mCycles[0];
        mOffset = freq / mFreq * (mPrevTime + mOffset) - mPrevTime;
    }
}
} // namespace EAGLAnim
