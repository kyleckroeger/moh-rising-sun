#include "SceneNode.h"
void IMovingSceneNode::Halt() {}
void IMovingSceneNode::PreTransform(const CMatrix &) {}
void IMovingSceneNode::Transform(const CMatrix &) {}
void IMovingSceneNode::Move(const CVector3 &) {}
void IMovingSceneNode::Rotate(const CVector3 &, float) {}
void IMovingSceneNode::Pitch(float) {}
void IMovingSceneNode::Roll(float) {}
void IMovingSceneNode::Yaw(float) {}
void IMovingSceneNode::SetPosition(const CVector3 &) {}
void IMovingSceneNode::SetBasis(const CVector3 &, const CVector3 &, const CVector3 &) {}
void IMovingSceneNode::Orthonormalize() {}
IMovingSceneNode *IMovingSceneNode::AsMovingNode() { return this; }
const IMovingSceneNode *IMovingSceneNode::AsMovingNode() const { return this; }
