#include "Light.h"
void CLight::Reset() { localToWorld.GetSlot(0); }
void CLight::PreTransform(const CMatrix &matrix) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(matrix, CMatrix::s_TempMat);
}
void CLight::Transform(const CMatrix &matrix) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(CMatrix::s_TempMat, matrix);
}
void CLight::SetTMLocalToWorld(const CMatrix &matrix) { localToWorld = matrix; }
void CLight::SetPosition(const CVector3 &position) { localToWorld.SetPos(position); }
void CLight::SetBasis(const CVector3 &right, const CVector3 &front, const CVector3 &up) {
    localToWorld.SetRight(right);
    localToWorld.SetFront(front);
    localToWorld.SetUp(up);
}
void CLight::Move(const CVector3 &offset) { localToWorld.Translate(offset); }
void CLight::Rotate(const CVector3 &axis, float angle) { localToWorld.Rotate(axis, angle); }
