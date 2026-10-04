// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
#include "EAGLLoading.h"
namespace EAGL {
SymbolPool::SymbolPool() {
    mTableLength = INIT_TABLE_SIZE;
    mTableSize = 0;
    mpSymbolTable = 0;
    mpFunctions = 0;
}
void SymbolPool::FreeMemory() {
    Empty();
    if (mpSymbolTable) {
        EAGLInternal::EAGLFree(mpSymbolTable, mTableLength * sizeof(*mpSymbolTable));
        mpSymbolTable = 0;
    }
}
SymbolPool::~SymbolPool() {
    Empty();
    if (mpSymbolTable) {
        EAGLInternal::EAGLFree(mpSymbolTable, mTableLength * sizeof(*mpSymbolTable));
        mpSymbolTable = 0;
    }
}
const char *SymbolPool::AddSymbol(const char *Name, void *Address) {
    EAGLInternal::SymbolEntry *p = (EAGLInternal::SymbolEntry *)EAGLInternal::EAGLMalloc(
        strlen(Name) + sizeof(*p) + 1, "EAGL::SymbolEntry");
    p->Address = Address;
    strcpy((char *)(p + 1), Name);
    Insert(Name, p);
    return (char *)(p + 1);
}
void SymbolPool::Empty() {
    if (mpSymbolTable) {
        for (unsigned int i = 0; i < mTableLength; i++) {
            EAGLInternal::SymbolEntry *p = mpSymbolTable[i];
            if (p)
                EAGLInternal::EAGLFree(p, strlen((char *)(p + 1)) + sizeof(*p) + 1);
            mpSymbolTable[i] = 0;
        }
    }
    mTableSize = 0;
    while (mpFunctions) {
        EAGLInternal::FunctionEntry *p = mpFunctions;
        mpFunctions = p->Next;
        EAGLInternal::EAGLFree(p, sizeof(*p));
    }
    mpFunctions = 0;
}
} // namespace EAGL
