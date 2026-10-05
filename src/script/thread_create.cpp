#include "ScriptRuntime.h"
int BSCreateThread(BSObject *object, BSCode_struct *level, BSMachineThread_struct *thread, void *context) {
    thread->context = context;
    thread->flags = 0;
    thread->level = level;
    thread->stackTop = g_piStackTop + 1;
    thread->frame = thread->stackTop + 1;
    thread->stateId = level->initialState;
    thread->state = level->states[thread->stateId];
    unsigned int code = thread->state->code;
    thread->instruction = level->codeBases[code >> 24] + (code & 0x00ffffff);
    thread->messages = 0;
    int count = thread->state->messageCount;
    BSCode_MessageHandlerEntry_struct *message = thread->state->messages;
    for (int i = 0; i < count; ++i) {
        BSMessageRegistration_struct *registration;
        BSMessageRegistration_struct *handler = BSRegisterMessage(&message[i], object, thread->stateId, thread, &registration);
        BSMessageListView *entry = BSMachineGetFreeMessageList();
        entry->next = thread->messages;
        entry->registration = registration;
        entry->handler = handler;
        entry->depth = thread->state->depth;
        entry->stateId = thread->stateId;
        thread->messages = entry;
    }
    BSExecuteThread(object, thread);
    return 0;
}
