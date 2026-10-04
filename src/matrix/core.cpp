// Reconstructed from the pinned target; see docs/Matrix.md.
#include "game/CMatrix.h"

extern "C" void* memcpy(void*, const void*, unsigned int);
extern float _Mat_Data[4][4];
extern CMatrix _Mat_Unit;

void CMatrix::InitClass() {
    memcpy(&_Mat_Unit, _Mat_Data, sizeof(CMatrix));
    s_ClassInit = 1;
}

CMatrix& CMatrix::operator=(const CMatrix& other) {
    row[0] = other.row[0];
    row[1] = other.row[1];
    row[2] = other.row[2];
    row[3] = other.row[3];
    return *this;
}

void CMatrix::BuildScale(float scale) {
    GetSlot(0);
    row[0].x = row[1].y = row[2].z = scale;
}

void CMatrix::BuildScale(const CVector3& scale) {
    GetSlot(0);
    row[0].x = Vector3Components(scale)[0];
    row[1].y = Vector3Components(scale)[1];
    row[2].z = Vector3Components(scale)[2];
}
