// AI-assisted reconstruction; evidence and scope: docs/Matrix.md.
#include "CMatrix.h"
void CMatrix::Orthographic(float x, float y, float near, float far) {
    float inv = 1.0f / (far - near);
    GetSlot(0);
    row[0].x = 1.0f / x;
    row[1].y = 1.0f / y;
    row[2].z = 2.0f * inv;
    row[3].z = (far + near) * inv;
    row[3].w = 0.0f;
}
