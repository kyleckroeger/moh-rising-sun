// Reconstructed from the pinned target; see docs/Matrix.md.
#include "game/CMatrix.h"

void MathSinCos(int angle, float* sine, float* cosine);

void CMatrix::BuildRotX(float angle) {
    float s, c;
    MathSinCos((int)(angle * 2670176.75f), &s, &c);
    GetSlot(0);
    row[1].y = c;
    row[1].z = s;
    row[2].y = -s;
    row[2].z = c;
}

void CMatrix::BuildRotY(float angle) {
    float s, c;
    MathSinCos((int)(angle * 2670176.75f), &s, &c);
    GetSlot(0);
    row[2].z = c;
    row[2].x = s;
    row[0].z = -s;
    row[0].x = c;
}

void CMatrix::BuildRotZ(float angle) {
    float s, c;
    MathSinCos((int)(angle * 2670176.75f), &s, &c);
    GetSlot(0);
    row[0].x = c;
    row[0].y = s;
    row[1].x = -s;
    row[1].y = c;
}
