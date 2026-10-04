#include "ScriptMachine.h"

void BSOpCodeFunc_AND() {
    *reinterpret_cast<unsigned int *>(g_piStackTop - 1) =
        g_piStackTop[-1] && *g_piStackTop;
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_OR() {
    *reinterpret_cast<unsigned int *>(g_piStackTop - 1) =
        g_piStackTop[-1] || *g_piStackTop;
    --g_piStackTop;
    ++g_piIP;
}
