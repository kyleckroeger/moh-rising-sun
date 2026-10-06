#include "SceneNode.h"
#include "CVector3.h"
void ISceneNode::GetRightward(CVector3 &axis) const { axis.Set(1.0f, 0.0f, 0.0f); }
void ISceneNode::GetForward(CVector3 &axis) const { axis.Set(0.0f, 1.0f, 0.0f); }
void ISceneNode::GetUpward(CVector3 &axis) const { axis.Set(0.0f, 0.0f, 1.0f); }
