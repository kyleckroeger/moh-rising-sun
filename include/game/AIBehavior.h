// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_AI_BEHAVIOR_H
#define GAME_AI_BEHAVIOR_H
#include "AITargeting.h"
class CThrownBullet;
class TriggerObject;

// Method-only views of the behaviors that script built-ins reach through
// CAIObject::FindMatchingBehavior. Each class's GetType returns the selector
// noted beside it. Storage, bases and virtual interfaces are not declared; the
// built-ins convert the returned behavior pointer without adjustment.
class CBuddyLeadBehavior { // Type 1.
  public:
    void HandleCheckpointReachedByBuddy(int);
    static void HandleCheckpointReachedByPlayer(int);
};

class CBuddyFollowBehavior { // Type 2.
  public:
    void CantReachTarget();
    void FinishedMovingAway();
    void SetDistances(float, float, float, float);
    void EnterBuddyTeleportArea(int);
    void ExitBuddyTeleportArea(int);
    int IsBuddyInArea(int);
    void UpdateDistanceToPlayer();
    static void RegisterBuddyFollowObject(BSObject *, float, float, float, float);
};

class CMountedMachineGunBehavior { // Type 3.
  public:
    void Arrived();
    void CantReachMMG();
    void HitFromBehind();
    void ForceToMMG();
    void ForceToMMGFeeder();
    static void DeregisterGun(int);
    static int GetFeederByLinkCode(int);
    static void RegisterFeeder(int, BSObject *);
};

class CCoverpointBehavior { // Type 4.
  public:
    int StartCoverPointWalk();
    void StopCoverPointWalk();
    void Arrived();
    void EndCoverpoint();
    void ForceToCoverPoint();
    static void InvalidateCoverPointByID(int);
    static void RegisterCoverpoint(TriggerObject *);
};

class CScriptedBehavior { // Type 5.
  public:
    void SetAllowedHighLevelState(EAIHighLevelState);
    void SetCapability(ECapability, bool);
    void StartScript(BSObject *);
    void StopScript();
};

class CSwordBehavior { // Type 8.
  public:
    int IsSwordDrawn();
};

class CGrenadeResponseBehavior { // Type 10.
  public:
    void FinishedResponse(CThrownBullet *);
};

// Method-only view of a cover point; the script object's native trigger is one.
class CCoverPoint {
  public:
    void Invalidate();
};

// Static lookup used by BIFunc_AIFindTank; script object at +0x0c as in CAIObject.
class CAITankObject {
  public:
    unsigned char unknown00[0x0c];
    BSObject *scriptObject;
    static CAITankObject *FindTank();
};

#endif
