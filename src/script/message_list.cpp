#include "ScriptRuntime.h"

int BSInitMessageFreeList() {
    g_MessageList = reinterpret_cast<BSMessageListView *>(g_pBSMachineMemory + g_pBSMachineMemoryOffset);
    g_pBSMachineMemoryOffset += g_iBSMessageRegistrationListSize * sizeof(BSMessageListView);
    for (int i = 0; i < g_iBSMessageRegistrationListSize - 1; ++i)
        g_MessageList[i].next = &g_MessageList[i + 1];
    g_MessageList[g_iBSMessageRegistrationListSize - 1].next = 0;
    g_MessageListHead = g_MessageList;
    return 0;
}

BSMessageListView *BSMachineGetFreeMessageList() {
    BSMessageListView *entry = g_MessageListHead;
    if (entry)
        g_MessageListHead = entry->next;
    return entry;
}
