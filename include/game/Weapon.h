// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_WEAPON_H
#define GAME_WEAPON_H

// Member-only view of CWeapon for the reload built-ins. This does not establish
// the complete allocation, base classes or virtual interface.
// Observed fields of the weapon's property record; not a complete layout.
struct WeaponProperties {
    unsigned char unknown_0[6];
    short refireRate; // +0x06
    unsigned char unknown_8[6];
    short projectile; // +0x0e; nonzero for projectile weapons.
};

class CWeapon {
  public:
    unsigned char unknown_0[0x300];
    WeaponProperties *properties; // +0x300
    unsigned char unknown_304[0x350 - 0x304];
    unsigned int reloading : 1; // +0x350, bit 31: cleared by BIFunc_PlayerClearReloading.
    unsigned int unknown_350 : 31;
    void StartReloading();
    void ReloadSingle();
    void DoneReloading();
};

#endif
