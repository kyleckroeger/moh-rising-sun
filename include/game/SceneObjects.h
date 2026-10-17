// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCENE_OBJECTS_H
#define GAME_SCENE_OBJECTS_H
#include "SceneNode.h"
struct BSObject;

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
class CStaticObject {
  public:
    struct Flags {
        unsigned int unknown_31_25 : 7;
        unsigned int allowCollisionDamage : 1; // Bit 24.
        unsigned int unknown_low : 24;
    };
    unsigned char unknown_0[0x218];
    Flags flags; // +0x218
};

class CAnimObject {
  public:
    unsigned char unknown_0[0x294];
    int gravity;    // +0x294
    int groundCheck; // +0x298
};

class CSoldierObject {
  public:
    unsigned char unknown_0[0x120];
    BSObject *scriptObject; // +0x120
    unsigned char unknown_124[0x220 - 0x124];
    int groundCheckPending; // +0x220
    unsigned char unknown_224[4];
    int groundCheckState;   // +0x228; cleared before the ground-check event.
    unsigned char unknown_22c[0xb14 - 0x22c];
    int canBePushed;        // +0xb14
    unsigned char unknown_b18[0x1148 - 0xb18];
    int aimSuspended;       // +0x1148; set by BIFunc_SuspendAiming.
    unsigned char unknown_114c[0x12b4 - 0x114c];
    int propAimSuspended;   // +0x12b4; set by BIFunc_SuspendPropAiming.
    unsigned char unknown_12b8[0x15e8 - 0x12b8];
    int japanese;           // +0x15e8; set by BIFunc_SetJapanese.
    void SetHeadTrans(bool);
    void ResumeAimModifier();
    void ResumePropAimModifier();
    void EnablePropAimModifier(bool);
    void SaveGagState();
    void SpecifyTurnAngle(float);
    void SetCurrentWeapon(int, int, bool);
    void FireCurrentWeapon(float, EFireMethod);
};

// Static decal operations used by the script built-ins.
class CDecalManager {
  public:
    static void ClearDecals();
};

#endif
