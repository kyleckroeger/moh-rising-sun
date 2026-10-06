// AI-assisted scoped reconstruction from GR8E69; see docs/Bullets.md.
#ifndef GAME_BULLET_H
#define GAME_BULLET_H
#pragma interface
#include "CMatrix.h"
#include "SceneNode.h"

class CCollision;
class CProjectileBullet;
class CThrownBullet;

// CBullet's table (0x802e9eb0) follows the IMovingSceneNode slot map and adds slots
// 89-102 in this order. Field names are descriptive; unlisted storage is unknown.
class CBullet : public IMovingSceneNode {
  public:
    enum EXPLOSION_PARTICLE_TYPE { ExplosionParticleTypeUnknown = 0 };

    unsigned char unknown_18[0x40 - 0x18];
    EClsnId collisionId;
    unsigned char unknown_44[0x70 - 0x44];
    CMatrix localToWorld;
    float field_b0, field_b4;

    void Draw(CDrawContext &);
    unsigned int IsDrawEnabled() const;
    void GetPosition(CVector3 &) const;
    void GetRightward(CVector3 &) const;
    void GetForward(CVector3 &) const;
    void GetTMLocalToWorld(CMatrix &) const;
    CBullet *AsBullet();
    const CBullet *AsBullet() const;

    virtual void Init(bool);
    virtual void Shutdown();
    virtual void GetUp(CVector3 &) const;
    virtual float GetDamage() const;
    virtual int GetScriptBulletType() const;
    virtual ISceneNode *GetFiredBy() const;
    virtual CThrownBullet *AsThrown();
    virtual CProjectileBullet *AsProjectile();
    virtual void GetVelocity(CVector3 &);
    virtual void SetExclusionPair(ISceneNode *);
    virtual void Penetrate(const CCollision &);
    virtual void SetDamage(float);
    virtual void SetBlastRadius(float);
    virtual void SetExplosionParticleSystem(unsigned long, EXPLOSION_PARTICLE_TYPE);
};

#endif
