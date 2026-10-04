#include "ScriptRuntime.h"

void BSOpCodeFunc_LS() {
    int value = g_pBSObject->scriptClass->sharedValues[BSInstructionArgument()];
    *++g_piStackTop = value;
    ++g_piIP;
}

void BSOpCodeFunc_GP() {
    unsigned int instruction = *g_piIP;
    int field = (instruction >> 16) & 0xff;
    TriggerObject *object;
    if (instruction >> 24)
        object = reinterpret_cast<TriggerObject *>(g_piFrame[3]);
    else
        object = g_pBSObject->nativeObject;
    ++g_piStackTop;
    *g_piStackTop = object->GetLegacyField(field);
    ++g_piIP;
}
