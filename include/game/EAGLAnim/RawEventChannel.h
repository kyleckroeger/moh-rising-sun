// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_RAW_EVENT_CHANNEL_H
#define MOH_RAW_EVENT_CHANNEL_H
#pragma interface
#include "AnimCore.h"
#include "EventHandler.h"
namespace EAGLAnim {
class RawEventChannel : public AnimMemoryMap {
  public:
    int GetNumEvents() const { return mNumEvents; }
    Event *GetEvents() { return reinterpret_cast<Event *>(this + 1); }
    void Eval(float, float, int &, float &, EventHandler **, void *);

  private:
    int mNumEvents;
};
} // namespace EAGLAnim
#endif
