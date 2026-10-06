#include "SceneNode.h"
int ISceneNode::IsVisible(CDrawContext &) const { return 0; }
unsigned int ISceneNode::IsDrawEnabled() const { return 0; }
void ISceneNode::SetDrawEnabled(bool) {}
EClsnId ISceneNode::GetCollisionId() const { return static_cast<EClsnId>(-1); }
void ISceneNode::SetCollisionId(EClsnId) {}
int ISceneNode::GetScriptObject() const { return 0; }
void ISceneNode::TriggerScriptEvent(int, void *, bool) {}
void ISceneNode::HandleBulletCollision(CBullet *, const CCollision &) {}
IMovingSceneNode *ISceneNode::AsMovingNode() { return 0; }
const IMovingSceneNode *ISceneNode::AsMovingNode() const { return 0; }
CStaticObject *ISceneNode::AsStaticObject() { return 0; }
const CStaticObject *ISceneNode::AsStaticObject() const { return 0; }
CHierObject *ISceneNode::AsHierObject() { return 0; }
const CHierObject *ISceneNode::AsHierObject() const { return 0; }
CWorldObject *ISceneNode::AsWorldObject() { return 0; }
const CWorldObject *ISceneNode::AsWorldObject() const { return 0; }
CAnimObject *ISceneNode::AsAnimObject() { return 0; }
const CAnimObject *ISceneNode::AsAnimObject() const { return 0; }
CSoldierObject *ISceneNode::AsSoldierObject() { return 0; }
const CSoldierObject *ISceneNode::AsSoldierObject() const { return 0; }
CPlayerObject *ISceneNode::AsPlayerObject() { return 0; }
const CPlayerObject *ISceneNode::AsPlayerObject() const { return 0; }
CAnimatedPlayer *ISceneNode::AsAnimatedPlayerObject() { return 0; }
const CAnimatedPlayer *ISceneNode::AsAnimatedPlayerObject() const { return 0; }
CPlayerWeaponObject *ISceneNode::AsPlayerWeaponObject() { return 0; }
const CPlayerWeaponObject *ISceneNode::AsPlayerWeaponObject() const { return 0; }
CLight *ISceneNode::AsLight() { return 0; }
const CLight *ISceneNode::AsLight() const { return 0; }
CProjector *ISceneNode::AsProjector() { return 0; }
const CProjector *ISceneNode::AsProjector() const { return 0; }
CBullet *ISceneNode::AsBullet() { return 0; }
const CBullet *ISceneNode::AsBullet() const { return 0; }
CCollisionVolume *ISceneNode::AsCollisionVolume() { return 0; }
const CCollisionVolume *ISceneNode::AsCollisionVolume() const { return 0; }
CBotObject *ISceneNode::AsBotObject() { return 0; }
const CBotObject *ISceneNode::AsBotObject() const { return 0; }
CAIDoodad *ISceneNode::GetAIDoodad() { return 0; }
const CAIDoodad *ISceneNode::GetAIDoodad() const { return 0; }
CAIObject *ISceneNode::GetAIObject() { return 0; }
void ISceneNode::SetAttachedLight(CLight *, const CVector3 &) {}
CLight *ISceneNode::GetAttachedLight() const { return 0; }
void ISceneNode::SerializeSaveData() {}
