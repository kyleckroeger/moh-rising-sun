#include "ScriptRuntime.h"
void BSMessageExecuteHandler(BSMessageRegistration_struct *registration, BSObject *sender, BSObject *target, TriggerObject *nativeObject, void *context) {
    BSObject *receiver = BSGetObjectByIndex(registration->objectIndex);
    BSMachineThread_struct *thread = BSGetThreadByIndex(registration->threadIndex, receiver);
    thread->stackTop = g_piStackTop + 2;
    thread->frame = thread->stackTop;
    *reinterpret_cast<void **>(thread->stackTop) = context;
    *reinterpret_cast<BSObject **>(thread->stackTop + 1) = sender;
    *reinterpret_cast<BSObject **>(thread->stackTop + 2) = target;
    *reinterpret_cast<TriggerObject **>(thread->stackTop + 3) = nativeObject;
    thread->stackTop += 3;
    unsigned int code = registration->message->code;
    thread->instruction = thread->level->codeBases[code >> 24] + (code & 0x00ffffff);
    BSExecuteThread(receiver, thread);
}
