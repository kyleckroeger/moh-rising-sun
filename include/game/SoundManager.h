// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SOUND_MANAGER_H
#define GAME_SOUND_MANAGER_H

// Method-only view of the embedded sound schedule.
class SoundSchedule {
  public:
    void ActivateBuss(unsigned int);
    void RemoveSound(unsigned int);
};

// Scoped view of the 0x8700-byte g_SoundMan object; its class name is not
// recovered. Only the members the script built-ins touch are named.
struct SoundManagerView {
    unsigned char unknown_0[0x837c];
    SoundSchedule schedule; // +0x837c
    unsigned char unknown_837d[0x85f4 - 0x837d];
    int soundZone;          // +0x85f4; set by BIFunc_SetSoundZone.
};
extern SoundManagerView g_SoundMan;

// Method-only view of the NPC vocal manager singleton.
class NPCVocalMan {
  public:
    void RemoveVocal(void *, int);
};
extern NPCVocalMan g_npcVocalMan;

#endif
