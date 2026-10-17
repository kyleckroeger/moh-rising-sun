// AI-assisted scoped reconstruction from GR8E69; see docs/CameraShake.md and docs/Paths.md.
#ifndef GAME_CAMERA_SHAKE_H
#define GAME_CAMERA_SHAKE_H
#pragma interface
class CAISplinePath;
class CWeapon;

// Scoped member view of CPlayerObject for shake and path controls. This does not
// establish the complete allocation, base classes or virtual interface.
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
        unsigned int unknown_low : 22;
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
    unsigned char unknown_e10[0x1cf0 - 0xe10];
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
};

#endif
