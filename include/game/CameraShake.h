// AI-assisted scoped reconstruction from GR8E69; see docs/CameraShake.md and docs/Paths.md.
#ifndef GAME_CAMERA_SHAKE_H
#define GAME_CAMERA_SHAKE_H
#pragma interface
class CAISplinePath;
class CWeapon;

// Scoped member view of CPlayerObject for shake and path controls. This does not
// establish the complete allocation, base classes or virtual interface.
// Observed fields of the player's embedded clip-plane settings at +0x1150.
struct ClipSettings {
    unsigned char unknown_0[0x38];
    float farClip;      // +0x38
    unsigned char unknown_3c[0x158 - 0x3c];
    int field_158;      // Cleared with field_15c by BIFunc_ChangeClipPlane.
    int field_15c;
    int changed;        // +0x160; set to 1 by BIFunc_ChangeClipPlane.
};

// Original enum tag; its members are unknown.
enum _LetterboxTransitionStyle { LetterboxTransitionStyleUnknown = 0 };

// Method-only view of the player's embedded heads-up display.
class CHeadsUpDisplay {
  public:
    void AddHitDirection(unsigned int);
    void LetterboxTransition(float, _LetterboxTransitionStyle);
};

class CPlayerObject {
  public:
    // Member-only view of the embedded look-at controller at +0x1cf0.
    class LookAtTarget {
      public:
        void Disable();
    };
    // 36 bytes: eight floats, then the table pointer at +32 (the class declares its
    // data before its virtual destructor). Field names are descriptive.
    class CCameraShake {
      public:
        float current;     // Intensity now.
        float target;      // Twice the requested intensity.
        float scale;       // 1 / intensity at the last SetShake (1 when none).
        float field_0c;
        float fadeTime;    // Ramp time used when the duration expires.
        float duration;    // 100000 means no automatic stop.
        float elapsed;
        float rate;        // Intensity change per update step; 0 when settled.
        CCameraShake();
        virtual ~CCameraShake();
        void SetShake(float, float, float, float);
        void SetShake(float, float);
        void Evaluate(float &, float &);
        void Update(float);
    };

    // Member-only view; unknown bytes retain their original ownership.
    struct Flags {
        unsigned int unknown_31_27 : 5;
        unsigned int canCrouch : 1;     // Bit 26: BIFunc_PlayerCanCrouch.
        unsigned int motionShake : 1;   // Bit 25: set by DoMotionShake for a positive amount.
        unsigned int unknown_24 : 1;
        unsigned int wielding : 1;      // Bit 23: BIFunc_SetPlayerWielding.
        unsigned int triggerChecks : 1; // Bit 22: BIFunc_TogglePlayerVsTriggerChecks.
        unsigned int superAimMode : 1;  // Bit 21: BIFunc_PlayerSuperAimMode.
        unsigned int unknown_20_12 : 9;
        unsigned int invincible : 1;    // Bit 11: BIFunc_MakePlayerInvincible.
        unsigned int unknown_low : 11;
    };
    unsigned char unknown_0[0xd44];
    unsigned int pathFlags; // +0xd44; bit 19 enables path movement, bit 27 is set while aiming.
    unsigned char unknown_d48[4];
    Flags flags;
    unsigned char unknown_d50[0xd58 - 0xd50];
    CCameraShake cameraShake;
    CCameraShake backgroundShake;
    float motionShakeAmount, motionShakeRate, motionShakeTime;

    unsigned char unknown_dac[0xe00 - 0xdac];
    CAISplinePath *movePath, *lookPath; // +0xe00, +0xe04
    float pathParameter, pathRate; // +0xe08, +0xe0c
    unsigned char unknown_e10[0xe30 - 0xe10];
    float fov;               // +0xe30; set by BIFunc_SetGameFOV.
    unsigned char unknown_e34[0xe40 - 0xe34];
    int fovMode;             // +0xe40
    unsigned char unknown_e44[0x10bc - 0xe44];
    int cameraPitchUnhinged; // +0x10bc
    unsigned char unknown_10c0[0x1144 - 0x10c0];
    int body;                // +0x1144; returned by BIFunc_MPGetPlayerBody.
    unsigned char unknown_1148[0x1150 - 0x1148];
    ClipSettings clip;       // +0x1150
    unsigned char unknown_12b4[0x1410 - 0x1150 - sizeof(ClipSettings)];
    CHeadsUpDisplay hud;     // +0x1410
    unsigned char unknown_1411[0x1c28 - 0x1411];
    int letterbox;           // +0x1c28; set by BIFunc_DisplayLetterbox.
    unsigned char unknown_1c2c[0x1cf0 - 0x1c2c];
    LookAtTarget lookAt; // +0x1cf0
    void MoveOnPath(CAISplinePath *, CAISplinePath *, float);
    void StopPath();

    void SetCameraShake(float, float, float, float);
    void StartCameraShake(float, float);
    void StopCameraShake(float);
    void StartBackgroundCameraShake(float, float);
    void StopBackgroundCameraShake(float);
    void DoMotionShake(float, float);

    CWeapon *GetCurrentWeapon() const;
    void FireMyWeapon(float);
    void SetFallingDamageState(bool);
    void Respawn();
    int CanPlayerSwitchWeapon();
    int IsPlayerCrouching() const;
    int IsInvincible() const;
    void StartDeathSequence();
    void DropMPDumpObject();
    int CycleWeapon(int, int);
    int GetNumGrenades() const;
    void ThrowGrenade(float);
    void ResetGrenadeCount(int);
    void SetDetonationTimeForPlayerTurret(float);
    int SetWeaponEnable(int, bool);
};

#endif
