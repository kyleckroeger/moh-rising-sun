// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_EAGLANIM_EVENTHANDLER_H
#define MOH_EAGLANIM_EVENTHANDLER_H

#pragma interface

#include "Event.h"

namespace EAGLAnim {

class EventHandler {
  public:
    EventHandler() {}

    void SetSuccessor(EventHandler *h) { mpSuccessor = h; }

    EventHandler *GetSuccessor() { return mpSuccessor; }

    virtual void HandleEvent(float time, const Event &event, void *extraData);

  private:
    EventHandler *mpSuccessor; // offset 0x0, size 0x4
};

}; // namespace EAGLAnim

#endif
