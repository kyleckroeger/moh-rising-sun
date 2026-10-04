// Reconstructed from pinned GR8E69 using the attributed NFS Most Wanted reference.
// Original symbols establish method names; fields are descriptive reconstruction
// names. Storage extents and remaining unknowns are documented in docs/Loading.md.
#ifndef GAME_EAGL_LOADING_H
#define GAME_EAGL_LOADING_H
#include "EAGLMemory.h"
#include <string.h>
typedef void *(*DynamicUserCallback)(const char *, bool &);
namespace EAGLInternal {
struct SymbolEntry {
    void *Address;
};
struct FunctionEntry {
    DynamicUserCallback Address;
    FunctionEntry *Prev, *Next;
};
} // namespace EAGLInternal
namespace EAGL {
extern unsigned int INIT_TABLE_SIZE;
class SymbolPool {
  public:
    unsigned int mTableSize, mTableLength;
    EAGLInternal::SymbolEntry **mpSymbolTable;
    unsigned int unknown_c;
    EAGLInternal::FunctionEntry *mpFunctions;
    SymbolPool();
    ~SymbolPool();
    static void operator delete(void *p, unsigned int n) { EAGLInternal::EAGLFree(p, n); }
    void FreeMemory();
    unsigned int HashFunction(const char *Name) {
        int i = 0, val1 = 0;
        while (Name[i]) {
            val1 ^= (val1 << 5) ^ Name[i];
            i++;
        }
        return val1 % mTableLength;
    }
    void Insert(const char *, void *);
    const char *AddSymbol(const char *, void *);
    void RemoveSymbol(const char *);
    void *Search(const char *, bool &);
    void Empty();
};
class DynamicLoader;
typedef void (*Constructor)(void *, DynamicLoader *, const char *);
typedef void (*Destructor)(void *);
typedef void *(*RuntimeAllocConstructor)(const char *, DynamicLoader *, int &, bool &, const char *);
typedef void (*RuntimeAllocDestructor)(void *, int);
class ConstructorPool {
  public:
    SymbolPool conspool, despool;
    ~ConstructorPool();
    void RemoveType(const char *);
    void AddType(const char *, Constructor, Destructor);
    Constructor FindConstructor(const char *);
    Destructor FindDestructor(const char *);
    void FreeMemory();
};
class RuntimeAllocConstructorPool {
  public:
    SymbolPool conspool, despool;
    ~RuntimeAllocConstructorPool();
    void RemoveType(const char *);
    void AddType(const char *, RuntimeAllocConstructor, RuntimeAllocDestructor);
    RuntimeAllocConstructor FindConstructor(const char *);
    RuntimeAllocDestructor FindDestructor(const char *);
    void FreeMemory();
};
} // namespace EAGL
#endif
