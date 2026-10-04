// Reconstructed from the pinned target; see docs/Matrix.md.
#include "game/CMatrix.h"

void CMatrix::Multiply(const CMatrix& a, const CMatrix& b) {
    row[0].x = a.row[0].x * b.row[0].x + a.row[0].y * b.row[1].x + a.row[0].z * b.row[2].x + a.row[0].w * b.row[3].x;
    row[0].y = a.row[0].x * b.row[0].y + a.row[0].y * b.row[1].y + a.row[0].z * b.row[2].y + a.row[0].w * b.row[3].y;
    row[0].z = a.row[0].x * b.row[0].z + a.row[0].y * b.row[1].z + a.row[0].z * b.row[2].z + a.row[0].w * b.row[3].z;
    row[0].w = a.row[0].x * b.row[0].w + a.row[0].y * b.row[1].w + a.row[0].z * b.row[2].w + a.row[0].w * b.row[3].w;
    row[1].x = a.row[1].x * b.row[0].x + a.row[1].y * b.row[1].x + a.row[1].z * b.row[2].x + a.row[1].w * b.row[3].x;
    row[1].y = a.row[1].x * b.row[0].y + a.row[1].y * b.row[1].y + a.row[1].z * b.row[2].y + a.row[1].w * b.row[3].y;
    row[1].z = a.row[1].x * b.row[0].z + a.row[1].y * b.row[1].z + a.row[1].z * b.row[2].z + a.row[1].w * b.row[3].z;
    row[1].w = a.row[1].x * b.row[0].w + a.row[1].y * b.row[1].w + a.row[1].z * b.row[2].w + a.row[1].w * b.row[3].w;
    row[2].x = a.row[2].x * b.row[0].x + a.row[2].y * b.row[1].x + a.row[2].z * b.row[2].x + a.row[2].w * b.row[3].x;
    row[2].y = a.row[2].x * b.row[0].y + a.row[2].y * b.row[1].y + a.row[2].z * b.row[2].y + a.row[2].w * b.row[3].y;
    row[2].z = a.row[2].x * b.row[0].z + a.row[2].y * b.row[1].z + a.row[2].z * b.row[2].z + a.row[2].w * b.row[3].z;
    row[2].w = a.row[2].x * b.row[0].w + a.row[2].y * b.row[1].w + a.row[2].z * b.row[2].w + a.row[2].w * b.row[3].w;
    row[3].x = a.row[3].x * b.row[0].x + a.row[3].y * b.row[1].x + a.row[3].z * b.row[2].x + a.row[3].w * b.row[3].x;
    row[3].y = a.row[3].x * b.row[0].y + a.row[3].y * b.row[1].y + a.row[3].z * b.row[2].y + a.row[3].w * b.row[3].y;
    row[3].z = a.row[3].x * b.row[0].z + a.row[3].y * b.row[1].z + a.row[3].z * b.row[2].z + a.row[3].w * b.row[3].z;
    row[3].w = a.row[3].x * b.row[0].w + a.row[3].y * b.row[1].w + a.row[3].z * b.row[2].w + a.row[3].w * b.row[3].w;
}
