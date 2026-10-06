// Reconstructed from GR8E69 with Codex assistance; see docs/Camera.md.
#include "Camera.h"

void CCamera::PreTransform(const CMatrix &matrix) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(matrix, CMatrix::s_TempMat);
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::Reset() {
    localToWorld.GetSlot(0);
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::Transform(const CMatrix &matrix) {
    CMatrix::s_TempMat = localToWorld;
    localToWorld.Multiply(CMatrix::s_TempMat, matrix);
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::SetTMLocalToWorld(const CMatrix &matrix) {
    localToWorld = matrix;
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::SetPosition(const CVector3 &position) {
    localToWorld.SetPos(position);
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::SetBasis(const CVector3 &right, const CVector3 &front, const CVector3 &up) {
    localToWorld.SetRight(right);
    localToWorld.SetFront(front);
    localToWorld.SetUp(up);
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::Move(const CVector3 &offset) {
    localToWorld.Translate(offset);
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::Rotate(const CVector3 &axis, float angle) {
    localToWorld.Rotate(axis, angle);
    worldToClipValid = worldToCameraValid = 0;
}
