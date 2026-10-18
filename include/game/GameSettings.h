// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_GAME_SETTINGS_H
#define GAME_GAME_SETTINGS_H

// Per-player progress record: 0xc598 bytes, first observed at +0xbf54 of the
// profile. Bit n of each mask is set for level n.
struct ProgressRecord {
    int filmCanisters[2]; // +0x0, +0x4; indexed by canister type.
    int replayItems;      // +0x8
    unsigned char unknown_c[0xc598 - 0xc];
};

// Observed fields of the profile the settings object points to.
struct GameProfile {
    unsigned char unknown_0[0xbf54];
    ProgressRecord records[3];          // +0xbf54
    unsigned char unknown_3101c[0x31664 - 0x3101c];
    unsigned char mission;              // +0x31664
    unsigned char level;                // +0x31665
    unsigned char unknown_31666[0x321e0 - 0x31666];
    signed char currentRecord;          // +0x321e0
};

// Member view of the game-settings singleton; only the profile pointer is named.
class CGameSettings {
  public:
    GameProfile *profile;
    int SerializingIn() const;
    int SerializingOut() const;
    void AddCheat(int);
    void RegisterSavePoint(int, bool);
    void TriggerSavePointResponse(int, bool);
    int HasMedal(int) const;
};
extern CGameSettings g_GameSettings;

#endif
