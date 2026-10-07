// AI-assisted reconstruction; see docs/Matrix.md, "CVector3 layout".
#include "CVector3.h"
float CVector3::Dot(const CVector3 &o) const { return x * o.x + y * o.y + z * o.z; }
