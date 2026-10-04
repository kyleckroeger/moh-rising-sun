#include "ScriptMachine.h"

void BSOpCodeFunc_POP() {
    g_piStackTop -= BSInstructionArgument();
    ++g_piIP;
}

void BSOpCodeFunc_PUSH() {
    *++g_piStackTop = g_piIP[1];
    g_piIP += 2;
}

void BSOpCodeFunc_SW() {
    g_piFrame[BSInstructionArgument()] = *g_piStackTop--;
    ++g_piIP;
}

void BSOpCodeFunc_LW() {
    ++g_piStackTop;
    *g_piStackTop = g_piFrame[BSInstructionArgument()];
    ++g_piIP;
}

void BSOpCodeFunc_SG() {
    g_piGlobals[BSInstructionArgument()] = *g_piStackTop--;
    ++g_piIP;
}

void BSOpCodeFunc_LG() {
    ++g_piStackTop;
    *g_piStackTop = g_piGlobals[BSInstructionArgument()];
    ++g_piIP;
}

void BSOpCodeFunc_SM() {
    reinterpret_cast<int *>(*g_piStackTop)[BSInstructionArgument()] = g_piStackTop[-1];
    g_piStackTop -= 2;
    ++g_piIP;
}

void BSOpCodeFunc_LM() {
    *g_piStackTop = reinterpret_cast<int *>(*g_piStackTop)[BSInstructionArgument()];
    ++g_piIP;
}
