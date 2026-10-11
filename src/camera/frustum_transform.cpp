// AI-assisted reconstruction from GR8E69; see docs/Camera.md.
#include "Frustum.h"
#include <math.h>
static inline void Normalize(CVector3 &v) {
    float length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length != 0.0f) {
        float inverse = 1.0f/length;
        v.x *= inverse;
        v.y *= inverse;
        v.z *= inverse;
    }
}
void CFrustum::Transform(const CMatrix &matrix) {
    for (int i = 0; i < 10; ++i) {
        planes[i].Transform(matrix);
        Normalize(planes[i].normal);
    }
}
