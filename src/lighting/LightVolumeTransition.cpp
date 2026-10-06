// AI-assisted reconstruction; see docs/LightVolumes.md.
#include "LightVolumeManager.h"

void DebugMsg(const char *, ...);
void CLightVolumeManager::UpdateTransition(BPDLightVolume *outgoing) {
    if (previous) {
        DebugMsg("CLightVolumeManager::UpdateTransition():  Still transitioning!!!\n");
        if (previous == volumes[0])
            transitionRemaining = transitionDuration - transitionRemaining;
    } else {
        transitionRemaining = transitionDuration;
    }
    if (outgoing == 0)
        previous = CLight::GetDefaultLightVolume();
    else
        previous = outgoing;
}
