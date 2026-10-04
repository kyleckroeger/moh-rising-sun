#include "ScriptRuntime.h"

void BSOpCodeFunc_NEXTSTATE() {
    BSMachineThread_struct *thread = g_pbmtThread;
    int nextState = static_cast<unsigned short>(*g_piStackTop);
    unsigned int depth = g_ppsteStateEntries[nextState]->depth;
    int registerMessages = 1;
    int foundExit = 0;
    int leaveStates = depth <= g_ppsteStateEntries[thread->stateId]->depth;
    BSEventView *event = 0;
    BSStateView **states = thread->level->states;
    if (!(thread->flags & 1) && leaveStates) {
        int state = thread->stateId;
        BSStateView *stateEntry;
        while (state != 0xffff && !foundExit
                && (stateEntry = states[state])->depth >= depth) {
            int eventCount = stateEntry->eventCount;
            event = stateEntry->events;
            for (int i = 0; i < eventCount; ++event, ++i) {
                if (event->eventNumber == 1) {
                    if (event->flags & 1)
                        foundExit = 1;
                    break;
                }
                if (event->eventNumber > 1)
                    break;
            }
            state = states[state]->parent;
        }
    }
    if (!foundExit) {
        thread->flags &= ~1;
        if (thread->stateId != nextState) {
            if (leaveStates) {
                BSMessageListView **link = &thread->messages;
                BSMessageListView *entry = thread->messages;
                while (entry) {
                    BSMessageListView *next = entry->next;
                    if (entry->depth > depth
                            || (entry->depth == depth && entry->stateId != nextState)) {
                        *link = next;
                        BSMessageRemoveHandler(entry->handler);
                        entry->next = g_MessageListHead;
                        g_MessageListHead = entry;
                        if (entry->registration)
                            entry->registration->flags &= ~2;
                    } else {
                        if (entry->stateId == nextState)
                            registerMessages = 0;
                        link = &entry->next;
                    }
                    entry = next;
                }
            } else {
                registerMessages = 1;
            }
            thread->stateId = nextState;
            thread->state = g_ppsteStateEntries[nextState];
            if (registerMessages) {
                BSCode_MessageHandlerEntry_struct *message = thread->state->messages;
                int count = thread->state->messageCount;
                for (int i = 0; i < count; ++i) {
                    BSMessageRegistration_struct *registration;
                    BSMessageRegistration_struct *handler = BSRegisterMessage(
                        &message[i], g_pBSObject, nextState, thread, &registration);
                    BSMessageListView *entry = BSMachineGetFreeMessageList();
                    entry->next = thread->messages;
                    entry->registration = registration;
                    entry->handler = handler;
                    entry->depth = depth;
                    entry->stateId = nextState;
                    thread->messages = entry;
                }
            }
        }
        unsigned int code = thread->state->code;
        g_piIP = g_piCodeBase[code >> 24] + (code & 0x00ffffff);
        g_piStackTop = g_piFrame - 1;
    } else {
        thread->flags |= 1;
        g_piStackTop = g_piFrame + 1;
        unsigned int code = event->code;
        *reinterpret_cast<unsigned int *>(g_piStackTop - 1) = nextState;
        *reinterpret_cast<BSObject **>(g_piStackTop) = g_pBSObject;
        g_piIP = g_piCodeBase[code >> 24] + (code & 0x00ffffff);
    }
}
