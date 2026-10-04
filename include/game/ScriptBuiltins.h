#ifndef GAME_SCRIPT_BUILTINS_H
#define GAME_SCRIPT_BUILTINS_H

// Scoped view of the 16-byte GameCube runtime table, not the on-disc format.
// Names for accessed fields are reconstruction choices; unknown bytes stay opaque.
struct BSBuiltinView {
    void (*function)(int **, void *);
    unsigned char unknown04[6];
    short stackAdjustment;
    short argumentCount;
    unsigned char unknown0e[2];
};

extern BSBuiltinView *g_pBuiltInFunctions;
extern int g_iCurrentBIFIndex;

#endif
