// AI-assisted reconstruction; evidence and scope: docs/Matrix.md.
#include "CMatrix.h"
void CMatrix::Inverse(const CMatrix &in) {
    float pos = 0.0f, neg = 0.0f, temp;
    temp = in.row[0].x * in.row[1].y * in.row[2].z;
    if (temp > 0.0f)
        pos += temp;
    else
        neg += temp;
    temp = in.row[0].y * in.row[1].z * in.row[2].x;
    if (temp > 0.0f)
        pos += temp;
    else
        neg += temp;
    temp = in.row[0].z * in.row[1].x * in.row[2].y;
    if (temp > 0.0f)
        pos += temp;
    else
        neg += temp;
    temp = -in.row[0].z * in.row[1].y * in.row[2].x;
    if (temp > 0.0f)
        pos += temp;
    else
        neg += temp;
    temp = -in.row[0].y * in.row[1].x * in.row[2].z;
    if (temp > 0.0f)
        pos += temp;
    else
        neg += temp;
    temp = -in.row[0].x * in.row[1].z * in.row[2].y;
    if (temp > 0.0f)
        pos += temp;
    else
        neg += temp;
    float det = pos + neg;
    if (det == 0.0f || __builtin_fabsf(det / (pos - neg)) < 1.0e-15f)
        return;
    float inv = 1.0f / det;
    row[0].x = (in.row[1].y * in.row[2].z - in.row[1].z * in.row[2].y) * inv;
    row[0].y = (in.row[0].z * in.row[2].y - in.row[0].y * in.row[2].z) * inv;
    row[0].z = (in.row[0].y * in.row[1].z - in.row[0].z * in.row[1].y) * inv;
    row[0].w = 0.0f;
    row[1].x = (in.row[1].z * in.row[2].x - in.row[1].x * in.row[2].z) * inv;
    row[1].y = (in.row[0].x * in.row[2].z - in.row[0].z * in.row[2].x) * inv;
    row[1].z = (in.row[0].z * in.row[1].x - in.row[0].x * in.row[1].z) * inv;
    row[1].w = 0.0f;
    row[2].x = (in.row[1].x * in.row[2].y - in.row[1].y * in.row[2].x) * inv;
    row[2].y = (in.row[0].y * in.row[2].x - in.row[0].x * in.row[2].y) * inv;
    row[2].z = (in.row[0].x * in.row[1].y - in.row[0].y * in.row[1].x) * inv;
    row[2].w = 0.0f;
    row[3].x = -(in.row[3].x * row[0].x + in.row[3].y * row[1].x + in.row[3].z * row[2].x);
    row[3].y = -(in.row[3].x * row[0].y + in.row[3].y * row[1].y + in.row[3].z * row[2].y);
    row[3].z = -(in.row[3].x * row[0].z + in.row[3].y * row[1].z + in.row[3].z * row[2].z);
    row[3].w = 1.0f;
}
