#include "SceneNode.h"
#include "CMatrix.h"
void ISceneNode::BeginUpdate(float) {}
void ISceneNode::UpdateAI(float) {}
void ISceneNode::CommitAI() {}
void ISceneNode::AttemptUpdate(float) {}
void ISceneNode::UpdateCollisionVolumes() {}
void ISceneNode::OnCollision(const CCollision &) {}
int ISceneNode::Constrain(float) { return 1; }
void ISceneNode::CommitUpdate() {}
void ISceneNode::PostUpdate(float) {}
void ISceneNode::Draw(CDrawContext &) {}
int ISceneNode::Attach(ISceneNode &, CMatrix *, unsigned int) { return 0; }
int ISceneNode::Detach() { return 0; }
int ISceneNode::GetLocalBoundingVolume(ISceneNode::EVolumeType) const { return 0; }
int ISceneNode::GetWorldBoundingVolume(ISceneNode::EVolumeType) const { return 0; }
void ISceneNode::GetTMLocalToWorld(CMatrix &matrix) const { matrix.GetSlot(0); }
