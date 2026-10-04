#include "ScriptRuntime.h"

unsigned char BSGetThreadIndex(BSMachineThread_struct *thread, BSObject *object) {
    for (int i = 0; i < object->scriptClass->levelCount; ++i)
        if (&object->threads[i] == thread)
            return i;
    return 0;
}

BSMachineThread_struct *BSGetThreadByIndex(unsigned char index, BSObject *object) {
    return &object->threads[index];
}
