#include "ScriptMachine.h"

void BSOpCodeFunc_GT() {
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] > g_piStackTop[0];
        break;
    case 0x0203:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) > static_cast<float>(g_piStackTop[0]);
        break;
    case 0x0302:
        g_piStackTop[-1] = static_cast<float>(g_piStackTop[-1]) > *reinterpret_cast<float *>(g_piStackTop);
        break;
    case 0x0303:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) > *reinterpret_cast<float *>(g_piStackTop);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_GTE() {
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] >= g_piStackTop[0];
        break;
    case 0x0203:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) >= static_cast<float>(g_piStackTop[0]);
        break;
    case 0x0302:
        g_piStackTop[-1] = static_cast<float>(g_piStackTop[-1]) >= *reinterpret_cast<float *>(g_piStackTop);
        break;
    case 0x0303:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) >= *reinterpret_cast<float *>(g_piStackTop);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_LT() {
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] < g_piStackTop[0];
        break;
    case 0x0203:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) < static_cast<float>(g_piStackTop[0]);
        break;
    case 0x0302:
        g_piStackTop[-1] = static_cast<float>(g_piStackTop[-1]) < *reinterpret_cast<float *>(g_piStackTop);
        break;
    case 0x0303:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) < *reinterpret_cast<float *>(g_piStackTop);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_LTE() {
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] <= g_piStackTop[0];
        break;
    case 0x0203:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) <= static_cast<float>(g_piStackTop[0]);
        break;
    case 0x0302:
        g_piStackTop[-1] = static_cast<float>(g_piStackTop[-1]) <= *reinterpret_cast<float *>(g_piStackTop);
        break;
    case 0x0303:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) <= *reinterpret_cast<float *>(g_piStackTop);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_EQ() {
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] == g_piStackTop[0];
        break;
    case 0x0203:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) == static_cast<float>(g_piStackTop[0]);
        break;
    case 0x0302:
        g_piStackTop[-1] = static_cast<float>(g_piStackTop[-1]) == *reinterpret_cast<float *>(g_piStackTop);
        break;
    case 0x0303:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) == *reinterpret_cast<float *>(g_piStackTop);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}

void BSOpCodeFunc_NE() {
    switch (static_cast<unsigned short>(*g_piIP >> 16)) {
    case 0x0202:
        g_piStackTop[-1] = g_piStackTop[-1] != g_piStackTop[0];
        break;
    case 0x0203:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) != static_cast<float>(g_piStackTop[0]);
        break;
    case 0x0302:
        g_piStackTop[-1] = static_cast<float>(g_piStackTop[-1]) != *reinterpret_cast<float *>(g_piStackTop);
        break;
    case 0x0303:
        g_piStackTop[-1] = *reinterpret_cast<float *>(g_piStackTop - 1) != *reinterpret_cast<float *>(g_piStackTop);
        break;
    default:
        g_piStackTop[-1] = 0;
        break;
    }
    --g_piStackTop;
    ++g_piIP;
}
