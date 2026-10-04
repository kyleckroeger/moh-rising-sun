#include "FlexProp.h"
void FlexProp::GetPosition(CVector3 &p) const {
    float *out = reinterpret_cast<float *>(&p);
    FlexPropFormat *f = format;
    out[0] = f->transform[9];
    out[1] = f->transform[10];
    out[2] = f->transform[11];
}
void FlexProp::SetPositionZ(float x) { format->transform[11] = x; }
