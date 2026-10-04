#include "ScriptMachine.h"

void BSOpCodeFunc_CAST() {
    union {
        float value;
        int bits;
    } result;
    int *top = g_piStackTop;
    switch (BSInstructionArgument()) {
    case 2:
        *top = static_cast<int>(*reinterpret_cast<float *>(top));
        break;
    case 3:
        result.value = static_cast<float>(*top);
        *top = result.bits;
        break;
    }
    ++g_piIP;
}
