// AI-assisted reconstruction; evidence and scope: docs/Matrix.md.
#include "CMatrix.h"
void MathSinCos(int, float *, float *);
void CMatrix::BuildRot(const CVector3 &axis, float angle) {
    float sine, cosine;
    MathSinCos((int)(angle * 2670176.75f), &sine, &cosine);
    const float *a = Vector3Components(axis);
    float x = a[0], y = a[1], z = a[2];
    float t = 1.0f - cosine;
    float tx = t * x, ty = t * y, tz = t * z;
    float txy = tx * y, txz = tx * z, tyz = ty * z;
    float sx = sine * x, sy = sine * y, sz = sine * z;
    row[0].x = tx * x + cosine;
    row[0].y = txy + sz;
    row[0].z = txz - sy;
    row[1].x = txy - sz;
    row[1].y = ty * y + cosine;
    row[1].z = tyz + sx;
    row[2].x = txz + sy;
    row[2].y = tyz - sx;
    row[2].z = tz * z + cosine;
    row[0].w = 0.0f;
    row[1].w = 0.0f;
    row[2].w = 0.0f;
    row[3].x = 0.0f;
    row[3].y = 0.0f;
    row[3].z = 0.0f;
    row[3].w = 1.0f;
}
