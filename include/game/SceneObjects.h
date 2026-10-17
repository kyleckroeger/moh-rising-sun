// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCENE_OBJECTS_H
#define GAME_SCENE_OBJECTS_H
#include "SceneNode.h"
struct BSObject;

// Original enum tag; its members are unknown.
enum EPlayerCollisionType { PlayerCollisionTypeUnknown = 0 };
EClsnId GetCollisionIDFromScriptCollisionType(EPlayerCollisionType);

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
    void SetHeadTrans(bool);
};

#endif
