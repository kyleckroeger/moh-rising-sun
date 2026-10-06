// AI-assisted scoped reconstruction; see docs/LightVolumes.md.
#ifndef MOH_LIGHTVOLUMEMANAGER_H
#define MOH_LIGHTVOLUMEMANAGER_H
#include "BPD.h"

// The seven words the manager methods access. Objects embed it (CAnimObject at
// +676, CStaticObject at +704); its complete size is not established.
// Member names are descriptive.
class CLightVolumeManager {
  public:
    BPDLightVolume *volumes[4];
    BPDLightVolume *previous;
    float transitionRemaining;
    float transitionDuration;
    CLightVolumeManager();
    ~CLightVolumeManager();
    void Reset();
    void Update(float);
    void AddVolume(BPDLightVolume *);
    void RemoveVolume(BPDLightVolume *);
    BPDLightVolume *GetVolume() const;
    void UpdateTransition(BPDLightVolume *);
};

#endif
