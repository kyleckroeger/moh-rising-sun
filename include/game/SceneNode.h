// AI-assisted scoped reconstruction; see docs/SceneNode.md.
#ifndef MOH_SCENENODE_H
#define MOH_SCENENODE_H
#pragma interface
#include "ObserverTypes.h"

class CBullet;
class CCollision;
class CDrawContext;
class CLight;
class CMatrix;
class CVector3;
class BPDLightVolume;
class IMovingSceneNode;
// Parameter enum; its members are unknown.
enum EClsnId { ClsnIdUnknown = 0 };

// Virtual slots 5-67, in the order of the original ISceneNode table at 0x802e1680.
// Names and parameter types come from original symbols. Return types are void
// placeholders unless an accepted override establishes one. Data members are
// not declared; complete size and layout remain unknown.
class ISceneNode : public IObserver {
  public:
    enum EVolumeType { VolumeTypeUnknown = 0 };
    virtual ~ISceneNode();
    virtual void BeginUpdate(float);
    virtual void UpdateAI(float);
    virtual void CommitAI();
    virtual void AttemptUpdate(float);
    virtual void UpdateCollisionVolumes();
    virtual void OnCollision(const CCollision &);
    virtual void Constrain(float);
    virtual void CommitUpdate();
    virtual void PostUpdate(float);
    virtual void Draw(CDrawContext &);
    virtual void Attach(ISceneNode &, CMatrix *, unsigned int);
    virtual void Detach();
    virtual void GetLocalBoundingVolume(ISceneNode::EVolumeType) const;
    virtual void GetWorldBoundingVolume(ISceneNode::EVolumeType) const;
    virtual void GetTMLocalToWorld(CMatrix &) const;
    virtual void GetWorldLinearVelocity() const;
    virtual void GetPosition(CVector3 &) const;
    virtual void GetCentroid(CVector3 &) const;
    virtual void GetRightward(CVector3 &) const;
    virtual void GetForward(CVector3 &) const;
    virtual void GetUpward(CVector3 &) const;
    virtual void IsVisible(CDrawContext &) const;
    virtual unsigned int IsDrawEnabled() const;
    virtual void SetDrawEnabled(bool);
    virtual void GetCollisionId() const;
    virtual void SetCollisionId(EClsnId);
    virtual void GetScriptObject() const;
    virtual void TriggerScriptEvent(int, void *, bool);
    virtual void HandleBulletCollision(CBullet *, const CCollision &);
    virtual IMovingSceneNode *AsMovingNode();
    virtual const IMovingSceneNode *AsMovingNode() const;
    virtual void AsStaticObject();
    virtual void AsStaticObject() const;
    virtual void AsHierObject();
    virtual void AsHierObject() const;
    virtual void AsWorldObject();
    virtual void AsWorldObject() const;
    virtual void AsAnimObject();
    virtual void AsAnimObject() const;
    virtual void AsSoldierObject();
    virtual void AsSoldierObject() const;
    virtual void AsPlayerObject();
    virtual void AsPlayerObject() const;
    virtual void AsAnimatedPlayerObject();
    virtual void AsAnimatedPlayerObject() const;
    virtual void AsPlayerWeaponObject();
    virtual void AsPlayerWeaponObject() const;
    virtual void AsLight();
    virtual void AsLight() const;
    virtual void AsProjector();
    virtual void AsProjector() const;
    virtual void AsBullet();
    virtual void AsBullet() const;
    virtual void AsCollisionVolume();
    virtual void AsCollisionVolume() const;
    virtual void AsBotObject();
    virtual void AsBotObject() const;
    virtual void GetAIDoodad();
    virtual void GetAIDoodad() const;
    virtual void GetAIObject();
    virtual void SetAttachedLight(CLight *, const CVector3 &);
    virtual void GetAttachedLight() const;
    virtual void SerializeSaveData();
};

// Virtual slots 68-88, introduced after ISceneNode. No IMovingSceneNode table is
// emitted; the order comes from derived tables such as IParticleSystem's.
class IMovingSceneNode : public ISceneNode {
  public:
    virtual void Reset();
    virtual void Halt();
    virtual void ApplyForceTo(const CVector3 &, float);
    virtual void SetTMLocalToWorld(const CMatrix &);
    virtual void PreTransform(const CMatrix &);
    virtual void Transform(const CMatrix &);
    virtual void Move(const CVector3 &);
    virtual void Rotate(const CVector3 &, float);
    virtual void Pitch(float);
    virtual void Roll(float);
    virtual void Yaw(float);
    virtual void SetPosition(const CVector3 &);
    virtual void SetBasis(const CVector3 &, const CVector3 &, const CVector3 &);
    virtual void Orthonormalize();
    virtual void EnterLightVolume(BPDLightVolume *);
    virtual void ExitLightVolume(BPDLightVolume *);
    virtual void GetLightVolume();
    virtual void SetLightVolumeTransitionDuration(float);
    virtual void GetName();
    virtual void GetTeam();
    virtual void GetPlayerIndex();
};

// Member-only view of the scene container; storage remains unknown.
class CScene {
  public:
    void Remove(ISceneNode &);
};
extern CScene g_scene;

#endif
