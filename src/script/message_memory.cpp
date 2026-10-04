#include "ScriptRuntime.h"

int BSMessageGetMemoryRequirements() {
    return g_iBSMessageRegistrationListSize * sizeof(BSMessageRegistration_struct) + 1776;
}
