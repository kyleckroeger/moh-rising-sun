#include "ScriptBuiltins.h"

void MUSIC_SetLevel(int);
void MUSIC_SendEvent(int);

void BIFunc_PathfinderSetLevel(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    MUSIC_SetLevel(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PathfinderEvent(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    MUSIC_SendEvent(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
