// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCENE_OBJECTS_H
#define GAME_SCENE_OBJECTS_H
#include "SceneNode.h"
struct BSObject;
class CWeapon;

// Original enum tag; its members are unknown.
enum EPlayerCollisionType { PlayerCollisionTypeUnknown = 0 };
EClsnId GetCollisionIDFromScriptCollisionType(EPlayerCollisionType);
// Original enum tag; its members are unknown.
enum EFireMethod { FireMethodUnknown = 0 };

// Member-only views of scene-object classes reached through the ISceneNode
// casts. Each view starts at the ISceneNode subobject (the casts return it
// unadjusted) and names only the observed members; bases, virtual interfaces
// and complete sizes are not declared. Built-ins that need an inherited virtual
// convert the view back to ISceneNode without adjustment.
// Method-only view of the soldier's embedded jaw controller.
enum EJawType { JawTypeUnknown = 0 };
class CJawController {
  public:
    void SetMotionType(EJawType);
};

class CStaticObject {
  public:
    void ResumeMotionPlayback();
    void FadeIn(float);
    void FadeOut(float);
    struct Flags {
        unsigned int unknown_31_25 : 7;
        unsigned int allowCollisionDamage : 1; // Bit 24.
        unsigned int unknown_low : 24;
    };
    unsigned char unknown_0[0x214];
    int maxLightPriority; // +0x214
    Flags flags;          // +0x218
};

class CAnimObject {
  public:
    unsigned char unknown_0[0x294];
    int gravity;    // +0x294
    int groundCheck; // +0x298
    void SetAttached(bool);
};

class CSoldierObject {
  public:
    // Original nested enum tag; members are descriptive.
    enum Partition { PartitionNone = 0, Partition2 = 2, Partition3 = 3 };
    unsigned char unknown_0[0x120];
    BSObject *scriptObject; // +0x120
    unsigned char unknown_124[0x220 - 0x124];
    int groundCheckPending; // +0x220
    unsigned char unknown_224[4];
    int groundCheckState;   // +0x228; cleared before the ground-check event.
    unsigned char unknown_22c[0x2ec - 0x22c];
    int locomotionInfo;     // +0x2ec; returned by address.
    unsigned char unknown_2f0[0xb14 - 0x2f0];
    int canBePushed;        // +0xb14
    unsigned char unknown_b18[0xb84 - 0xb18];
    CWeapon *weapons[1];    // +0xb84; slot table, indexed by slot number (extent unknown).
    unsigned char unknown_b88[0xf20 - 0xb88];
    int currentWeaponSlot;  // +0xf20
    unsigned char unknown_f24[0x1050 - 0xf24];
    int voiceHandle;        // +0x1050; -12 when no voice is playing.
    unsigned char unknown_1054[0x10a8 - 0x1054];
    CJawController jaw;     // +0x10a8
    unsigned char unknown_10a9[0x1148 - 0x10a9];
    int aimSuspended;       // +0x1148; set by BIFunc_SuspendAiming.
    unsigned char unknown_114c[0x12b4 - 0x114c];
    int propAimSuspended;   // +0x12b4; set by BIFunc_SuspendPropAiming.
    unsigned char unknown_12b8[0x15e8 - 0x12b8];
    int japanese;           // +0x15e8; set by BIFunc_SetJapanese.
    unsigned char unknown_15ec[0x1c54 - 0x15ec];
    float reloadTimer;      // +0x1c54; reset to -1 by BIFunc_BotDoneReloading.
    void SetHeadTrans(bool);
    void SetFaceAnimationState(unsigned short, int);
    void SetHeadTrackPlayer(bool);
    void EnableSkeletalPartition(Partition);
    void DisableSkeletalPartition(Partition);
    ISceneNode *GetAttachedObjectAt(char *);
    void ResumeAimModifier();
    void ResumePropAimModifier();
    void EnablePropAimModifier(bool);
    void SaveGagState();
    void SpecifyTurnAngle(float);
    void SetCurrentWeapon(int, int, bool);
    void FireCurrentWeapon(float, EFireMethod);
};

// Static animated-texture operations keyed by texture CRC.
class AnimShape {
  public:
    static void ResetByCRC(int);
    static void AdvanceByCRC(int);
    static void SetFrameByCRC(int, float);
    static void SetFrameRateByCRC(int, float);
};

// Static decal operations used by the script built-ins.
class CDecalManager {
  public:
    static void ClearDecals();
};

// Four-byte colour passed by value; channel names are descriptive.
class CColor {
  public:
    unsigned char r, g, b, a;
};

// Static screen-fade entry points.
class CFader {
  public:
    static void Flash(CColor, float, float);
    static void FadeIn(CColor, float);
    static void FadeOut(CColor, float);
};

// Virtual-interface view of the animated-object tables (slots 89-113 after the
// IMovingSceneNode slots), in the order of the original CSoldierObject table.
// Names come from the symbols in each slot; return and parameter types beyond
// the symbols are placeholders. It declares no storage: callers convert the
// scene-node pointers they already hold, which point at the same object.
namespace EALA { class Task; }
namespace EAGL { class Transform; }
enum EAnimCallback { AnimCallbackUnknown = 0 };
class CAnimObjectInterface : public IMovingSceneNode {
  public:
    virtual void Init();
    virtual void Shutdown();
    virtual void CleanUp();
    virtual int SetAnimationState(const char *, int);
    virtual int SetAnimationState(const char *, EALA::Task *);
    virtual void UpdateAnimation(float);
    virtual void SetMoveTarget(const CVector3 &);
    virtual void SetScript(BSObject *);
    virtual void UpdateLocalBoundingVolume(ISceneNode::EVolumeType, const CVector3 &, const CVector3 &);
    virtual int LineAlmostCollided(CBullet *) const;
    virtual void GetTMLocalToWorld(EAGL::Transform &) const;
    virtual int AttachObject(CStaticObject *, char *, int);
    virtual int DetachObject(char *);
    virtual int GetAttachPointMatrix(char *, CMatrix *);
    virtual void OnSwordDraw();
    virtual void OnSwordSheath();
    virtual void AttachFloatingBone();
    virtual void DetachFloatingBone();
    virtual void SetAnimCallback(EAnimCallback);
    virtual float FadeValue() const;
    virtual void FadeOut(float);
    virtual void FadeIn(float);
    virtual int AttachObject(int, char *, int);
    virtual void GetAimTarget(CVector3 &) const;
    virtual void EnableAimModifier(bool);
};

#endif
