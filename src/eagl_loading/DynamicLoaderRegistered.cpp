// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
#include "EAGLDynamicLoader.h"
namespace EAGL {
const char *DynamicLoader::RegisterVar(const char *name, void *addr) {
    return gSymbolPool.AddSymbol(name, addr);
}
void DynamicLoader::UnRegisterVar(const char *name) { gSymbolPool.RemoveSymbol(name); }
void *DynamicLoader::GetRegisteredVar(const char *name, bool &valid) {
    return gSymbolPool.Search(name, valid);
}
} // namespace EAGL
