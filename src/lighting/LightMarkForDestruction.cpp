#include "Light.h"
// Leave the scene first if still in it, then use the subject's countdown.
void CLight::MarkForDestruction(int countdown) {
    if (g_scene.IsNodeInScene(this))
        g_scene.Remove(*this);
    ISubject::MarkForDestruction(countdown);
}
