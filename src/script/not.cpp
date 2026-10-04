#include "ScriptMachine.h"

void BSOpCodeFunc_NOT() {
    *g_piStackTop = !*g_piStackTop;
    ++g_piIP;
}
