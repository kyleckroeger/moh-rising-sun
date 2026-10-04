// Reconstructed from the pinned target; see docs/Matrix.md.
#include "game/CMatrix.h"

void CMatrix::BuildTrans(const CVector3& position) {
    GetSlot(0);
    SetPos(position);
}

void CMatrix::SetRight(const CVector3& v) {
    row[0].x = Vector3Components(v)[0];
    row[0].y = Vector3Components(v)[1];
    row[0].z = Vector3Components(v)[2];
}

void CMatrix::SetFront(const CVector3& v) {
    row[1].x = Vector3Components(v)[0];
    row[1].y = Vector3Components(v)[1];
    row[1].z = Vector3Components(v)[2];
}

void CMatrix::SetUp(const CVector3& v) {
    row[2].x = Vector3Components(v)[0];
    row[2].y = Vector3Components(v)[1];
    row[2].z = Vector3Components(v)[2];
}

void CMatrix::SetPos(const CVector3& v) {
    row[3].x = Vector3Components(v)[0];
    row[3].y = Vector3Components(v)[1];
    row[3].z = Vector3Components(v)[2];
}

void CMatrix::PreTranslate(const CVector3& v) {
    row[3].x += Vector3Components(v)[0] * row[0].x + Vector3Components(v)[1] * row[1].x + Vector3Components(v)[2] * row[2].x;
    row[3].y += Vector3Components(v)[0] * row[0].y + Vector3Components(v)[1] * row[1].y + Vector3Components(v)[2] * row[2].y;
    row[3].z += Vector3Components(v)[0] * row[0].z + Vector3Components(v)[1] * row[1].z + Vector3Components(v)[2] * row[2].z;
}

void CMatrix::Translate(const CVector3& v) {
    row[3].x += Vector3Components(v)[0];
    row[3].y += Vector3Components(v)[1];
    row[3].z += Vector3Components(v)[2];
}
