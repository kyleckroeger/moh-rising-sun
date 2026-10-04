// Attributed reference names and target-verified storage; see docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_EAGLANIM_EVENT_H
#define MOH_EAGLANIM_EVENT_H

#pragma interface

namespace EAGLAnim {

// total size: 0x10
struct Event {
    int eventId;       // offset 0x0, size 0x4
    float triggerTime; // offset 0x4, size 0x4
    float duration;    // offset 0x8, size 0x4
    float parameter;   // offset 0xC, size 0x4
};

}; // namespace EAGLAnim

#endif
