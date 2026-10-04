// AI-assisted reconstruction; evidence and scope: docs/Matrix.md.
#include "CMatrix.h"
void CMatrix::Perspective(float x, float y, float near, float far) {
    float inv = 1.0f / (near * (far - near));
    float z = (far + near) * inv;
    float t = (near * (-2.0f * far)) * inv;
    GetSlot(0);
    row[0].x = 1.0f / (x * near);
    row[1].y = 1.0f / (y * near);
    row[2].z = z;
    row[2].w = 1.0f / near;
    row[3].z = t;
    row[3].w = 0.0f;
}
