#include "ScriptMachine.h"

void BSOpCodeFunc_NEG() {
    union {
        float value;
        int bits;
    } result;
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 2:
        *g_piStackTop = -*g_piStackTop;
        break;
    case 3:
        result.value = -*reinterpret_cast<float *>(g_piStackTop);
        *g_piStackTop = result.bits;
        break;
    default:
        *g_piStackTop = 0;
        break;
    }
    ++g_piIP;
}
