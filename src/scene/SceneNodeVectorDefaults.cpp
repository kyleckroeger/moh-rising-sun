#include "SceneNode.h"
#include "CVector3.h"
CVector3 ISceneNode::GetWorldLinearVelocity() const { return CVector3(0.0f); }
void ISceneNode::GetPosition(CVector3 &position) const { position.x = position.y = position.z = 0.0f; }
