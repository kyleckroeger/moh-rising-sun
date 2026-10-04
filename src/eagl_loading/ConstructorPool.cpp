// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
// Adapted from nfsmw 1f2cdd7996791c81a580b3f7b36b44d4f9f6719c (CC0).
#include "EAGLLoading.h"
namespace EAGL {
ConstructorPool::~ConstructorPool() {
    conspool.~SymbolPool();
    despool.~SymbolPool();
}
void ConstructorPool::RemoveType(const char *type) {
    conspool.RemoveSymbol(type);
    despool.RemoveSymbol(type);
}
void ConstructorPool::AddType(const char *type, Constructor c, Destructor d) {
    conspool.AddSymbol(type, (void *)c);
    despool.AddSymbol(type, (void *)d);
}
Constructor ConstructorPool::FindConstructor(const char *type) {
    bool valid;
    Constructor c = (Constructor)conspool.Search(type, valid);
    if (!valid)
        c = 0;
    return c;
}
Destructor ConstructorPool::FindDestructor(const char *type) {
    bool valid;
    Destructor d = (Destructor)despool.Search(type, valid);
    if (!valid)
        d = 0;
    return d;
}
RuntimeAllocConstructorPool::~RuntimeAllocConstructorPool() {
    conspool.~SymbolPool();
    despool.~SymbolPool();
}
void RuntimeAllocConstructorPool::RemoveType(const char *type) {
    conspool.RemoveSymbol(type);
    despool.RemoveSymbol(type);
}
void RuntimeAllocConstructorPool::AddType(const char *type, RuntimeAllocConstructor c,
                                          RuntimeAllocDestructor d) {
    conspool.AddSymbol(type, (void *)c);
    despool.AddSymbol(type, (void *)d);
}
RuntimeAllocConstructor RuntimeAllocConstructorPool::FindConstructor(const char *type) {
    bool valid;
    RuntimeAllocConstructor c = (RuntimeAllocConstructor)conspool.Search(type, valid);
    if (!valid)
        c = 0;
    return c;
}
RuntimeAllocDestructor RuntimeAllocConstructorPool::FindDestructor(const char *type) {
    bool valid;
    RuntimeAllocDestructor d = (RuntimeAllocDestructor)despool.Search(type, valid);
    if (!valid)
        d = 0;
    return d;
}
void RuntimeAllocConstructorPool::FreeMemory() {
    conspool.FreeMemory();
    despool.FreeMemory();
}
void ConstructorPool::FreeMemory() {
    conspool.FreeMemory();
    despool.FreeMemory();
}
} // namespace EAGL
