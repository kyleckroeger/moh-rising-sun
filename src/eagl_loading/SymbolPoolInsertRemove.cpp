// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
// Adapted from dbalatoni13/nfsmw 1f2cdd7996791c81a580b3f7b36b44d4f9f6719c, CC0.
#include "EAGLLoading.h"
namespace EAGL {
void SymbolPool::Insert(const char *Name, void *value) {
    unsigned int i;

    if (!mpSymbolTable) {
        mTableLength = EAGL::INIT_TABLE_SIZE;
        mpSymbolTable = reinterpret_cast<EAGLInternal::SymbolEntry **>(EAGLInternal::EAGLMalloc(
            EAGL::INIT_TABLE_SIZE * sizeof(*mpSymbolTable), "EAGL::SymbolPool::mpSymbolTable"));
        for (i = 0; i < mTableLength; i++) {
            mpSymbolTable[i] = 0;
        };
    }

    unsigned int index = HashFunction(Name);

    while (mpSymbolTable[index]) {
        index++;
        if (index >= mTableLength) {
            index = 0;
        }
    }
    mpSymbolTable[index] = reinterpret_cast<EAGLInternal::SymbolEntry *>(value);
    mTableSize++;
    if (mTableSize >= mTableLength) {
        const int oldTableLength = mTableLength;
        EAGLInternal::SymbolEntry **oldTable = mpSymbolTable;
        mTableLength = oldTableLength * 2;

        mpSymbolTable = reinterpret_cast<EAGLInternal::SymbolEntry **>(EAGLInternal::EAGLMalloc(
            mTableLength * sizeof(*mpSymbolTable), "EAGL::SymbolPool::mpSymbolTable"));

        for (i = 0; i < mTableLength; i++) {
            mpSymbolTable[i] = 0;
        }

        mTableSize = 0;
        for (int i = 0; i < oldTableLength; i++) {
            Insert(reinterpret_cast<const char *>(oldTable[i] + 1), oldTable[i]);
        }

        if (oldTable) {
            EAGLInternal::EAGLFree(oldTable, oldTableLength * sizeof(*oldTable));
        }
    }
}

void SymbolPool::RemoveSymbol(const char *Name) {
    if (!mpSymbolTable)
        return;
    unsigned int index = HashFunction(Name), count = 0;
    EAGLInternal::SymbolEntry *p;
    while (!(p = mpSymbolTable[index]) || strcmp(Name, (char *)(p + 1)) != 0) {
        if (count > mTableLength)
            return;
        index++;
        if (index >= mTableLength)
            index = 0;
        count++;
    }
    EAGLInternal::SymbolEntry *entry = mpSymbolTable[index];
    if (entry) {
        EAGLInternal::EAGLFree(entry, strlen((char *)(entry + 1)) + sizeof(*entry) + 1);
        mpSymbolTable[index] = 0;
        mTableSize--;
    }
}
} // namespace EAGL
