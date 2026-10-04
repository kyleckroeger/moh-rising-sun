#include "ScriptMachine.h"
#include "ScriptBuiltins.h"

void BSOpCodeFunc_FNCALL() {
    int *previousTop = g_piStackTop;
    int argumentCount = reinterpret_cast<unsigned char *>(g_piIP)[1];
    g_piStackTop += 2;
    previousTop[1] = reinterpret_cast<int>(g_piFrame);
    previousTop[2] = reinterpret_cast<int>(g_piIP + 2);
    g_piFrame = previousTop - argumentCount + 1;
    unsigned int target = g_piIP[1];
    g_piIP = g_piCodeBase[target >> 24] + (target & 0x00ffffff);
}

void BSOpCodeFunc_BIFNCALL() {
    g_iCurrentBIFIndex = BSInstructionArgument();
    g_pBuiltInFunctions[g_iCurrentBIFIndex].function(&g_piStackTop, g_pObj);
    ++g_piIP;
}

void BSOpCodeFunc_RETURN() {
    int *previousFrame = g_piFrame;
    int *saved = previousFrame + reinterpret_cast<unsigned char *>(g_piIP)[0];
    g_piFrame = reinterpret_cast<int *>(saved[0]);
    if (reinterpret_cast<unsigned char *>(g_piIP)[1]) {
        int *previousTop = g_piStackTop;
        g_piStackTop = previousFrame;
        *g_piStackTop = *previousTop;
    } else {
        g_piStackTop = previousFrame - 1;
    }
    g_piIP = reinterpret_cast<int *>(saved[1]);
}

void BSOpCodeFunc_BREAK() {
    g_iStopThread = 1;
    g_piIP += BSInstructionArgument();
}

void BSOpCodeFunc_TRACE() {
    ++g_piIP;
}
