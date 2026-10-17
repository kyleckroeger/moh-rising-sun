// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_GAME_SETTINGS_H
#define GAME_GAME_SETTINGS_H

// Method-only view of the game-settings singleton; storage remains original.
class CGameSettings {
  public:
    int SerializingIn() const;
    int SerializingOut() const;
};
extern CGameSettings g_GameSettings;

#endif
