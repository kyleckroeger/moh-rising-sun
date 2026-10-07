// AI-assisted scoped reconstruction from GR8E69; see docs/LightVolumes.md.
#ifndef GAME_VOLUME_H
#define GAME_VOLUME_H
#pragma interface
#include "CVector3.h"

class CDrawContext;

// Only the inline destructor is declared; the other IVolume virtual slots are
// not reproduced here. The destructor resets the table pointer, as the
// original _._7IVolume does.
class IVolume {
  public:
    virtual ~IVolume() {}
};

// 32 bytes (CVolSphere::Create allocates 32). The radius at +12 and center at
// +16 are established by GetRadius/SetRadius and GetExtents; +4 is unknown.
class CVolSphere : public IVolume {
  public:
    unsigned char unknown_04[8];
    float radius;
    CVector3 center;
    CVolSphere(float r, const CVector3 &c) : radius(r), center(c) {}
    int TestVisibility(CDrawContext &) const;
};

#endif
