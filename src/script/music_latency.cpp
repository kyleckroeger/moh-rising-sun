#include "ScriptBuiltins.h"

void MUSIC_SetLatency(int);

void BIFunc_PathfinderSetLatency(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    MUSIC_SetLatency(args[1]);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
