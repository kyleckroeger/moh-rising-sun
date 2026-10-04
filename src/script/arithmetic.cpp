#include "ScriptMachine.h"

// Descriptive literal-pool label: ADD uses a separate four-byte zero constant.
static const float addInitialValue[1]
    __attribute__((section(".rodata.add_initial"), aligned(4))) = { 0.0f };

void BSOpCodeFunc_ADD() {
    float result = addInitialValue[0];
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] + g_piStackTop[0];
        break;
    case 0x0203:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            + static_cast<float>(g_piStackTop[0]);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0302:
        result = static_cast<float>(g_piStackTop[-1])
            + *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0303:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            + *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_SUB() {
    float result = 0.0f;
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] - g_piStackTop[0];
        break;
    case 0x0203:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            - static_cast<float>(g_piStackTop[0]);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0302:
        result = static_cast<float>(g_piStackTop[-1])
            - *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0303:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            - *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_DIV() {
    float result = 0.0f;
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] / g_piStackTop[0];
        break;
    case 0x0203:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            / static_cast<float>(g_piStackTop[0]);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0302:
        result = static_cast<float>(g_piStackTop[-1])
            / *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0303:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            / *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_MULT() {
    float result = 0.0f;
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] * g_piStackTop[0];
        break;
    case 0x0203:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            * static_cast<float>(g_piStackTop[0]);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0302:
        result = static_cast<float>(g_piStackTop[-1])
            * *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    case 0x0303:
        result = *reinterpret_cast<float *>(g_piStackTop - 1)
            * *reinterpret_cast<float *>(g_piStackTop);
        g_piStackTop[-1] = *reinterpret_cast<int *>(&result);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}
