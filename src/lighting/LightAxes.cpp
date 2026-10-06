#include "Light.h"
void CLight::Orthonormalize() { localToWorld.Orthonormalize(); }
void CLight::GetTMLocalToWorld(CMatrix &out) const { out = localToWorld; }
void CLight::GetPosition(CVector3 &v) const { v = localToWorld.GetPos(); }
void CLight::GetRightward(CVector3 &v) const { v = localToWorld.GetRight(); }
void CLight::GetForward(CVector3 &v) const { v = localToWorld.GetFront(); }
void CLight::GetUpward(CVector3 &v) const { v = localToWorld.GetUp(); }
