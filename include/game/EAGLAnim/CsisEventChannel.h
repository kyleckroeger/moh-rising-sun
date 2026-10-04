// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_EAGLANIM_CSISEVENTCHANNEL_H
#define MOH_EAGLANIM_CSISEVENTCHANNEL_H

#pragma interface

#include "AnimCore.h"
#include "Csis.h"
#include "EventHandler.h"

namespace EAGLAnim {

// Twenty-byte serialized prefix; eight-byte event records follow immediately.
class CsisEventChannel : public AnimMemoryMap {
  public:
    int GetNumEvents() const { return mNumEvents; }

    void SetNumEvents(int n) { mNumEvents = n; }

    CsisEvent *GetEvents() { return reinterpret_cast<CsisEvent *>(&this[1]); }

    CsisDictionary *GetCsisDictionary() { return mCsisDict; }

    void SetCsisDictionary(CsisDictionary *dict) { mCsisDict = dict; }

    unsigned short *GetCsisDataIndex() { return mCsisDataIndex; }

    void SetCsisDataIndex(unsigned short *data) { mCsisDataIndex = data; }

    unsigned char *GetCsisData() { return mCsisData; }

    void SetCsisData(unsigned char *data) { mCsisData = data; }

    int GetSize() const;

    static int ComputeSize(int numEvents);

    void HandleEvents(float currentTime, EventHandler **eventHandlers, void *extraData, CsisEvent &event);

    void Eval(float previousTime, float currentTime, int &currentIdx, float &cacheCurrentTime,
              EventHandler **eventHandlers, void *extraData);

  private:
    int mNumEvents;                 // offset 0x4, size 0x4
    CsisDictionary *mCsisDict;      // offset 0x8, size 0x4
    unsigned short *mCsisDataIndex; // offset 0xC, size 0x4
    unsigned char *mCsisData;       // offset 0x10, size 0x4
};

}; // namespace EAGLAnim

#endif
