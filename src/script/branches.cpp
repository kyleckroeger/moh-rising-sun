#include "ScriptMachine.h"

void BSOpCodeFunc_BNE() {
    if (*g_piStackTop)
        ++g_piIP;
    else
        g_piIP += BSInstructionArgument();
    --g_piStackTop;
}

void BSOpCodeFunc_JMP() {
    g_piIP += BSInstructionArgument();
}
