// AI-assisted reconstruction; see docs/LightVolumes.md.
#include "profile.h"
#include <algorithm>
#include "LightVolumeManager.h"

CLightVolumeManager::CLightVolumeManager() { Reset(); }
CLightVolumeManager::~CLightVolumeManager() { Reset(); }
void CLightVolumeManager::Reset() {
    for (int i = 3; i >= 0; i--)
        volumes[i] = 0;
    previous = 0;
    transitionRemaining = 0.0f;
    transitionDuration = 10.0f;
}
void CLightVolumeManager::Update(float elapsed) {
    if (previous) {
        transitionRemaining -= elapsed;
        if (!(transitionRemaining > 0.0f))
            previous = 0;
    }
}
void CLightVolumeManager::AddVolume(BPDLightVolume *volume) {
    if (volumes[3] == 0) {
        BPDLightVolume *top = volumes[0];
        for (int i = 0; i < 4; i++) {
            if (volumes[i] == 0) {
                volumes[i] = volume;
                break;
            }
            if (volumes[i]->priority < volume->priority) {
                _STL::copy_backward(volumes + i, volumes + 3, volumes + 4);
                volumes[i] = volume;
                break;
            }
        }
        if (top != volumes[0])
            UpdateTransition(top);
    }
}
void CLightVolumeManager::RemoveVolume(BPDLightVolume *volume) {
    BPDLightVolume *top = volumes[0];
    for (int i = 0; i < 4; i++) {
        if (volumes[i] == volume) {
            volumes[i] = 0;
            for (int j = i + 1; j < 4 && volumes[j]; j++) {
                volumes[j - 1] = volumes[j];
                volumes[j] = 0;
            }
            if (top != volumes[0])
                UpdateTransition(top);
            return;
        }
    }
}
