// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
#include "EAGLDynamicLoader.h"
namespace EAGL {
inline int DynamicLoader::GetCount() const {
    if (!handle) {
        return 0;
    }
    HashPointer *h = reinterpret_cast<HashPointer *>(handle);
    return h->symbols_num;
}

bool DynamicLoader::GetNextSymbol(const char *type, int &iIndex, Symbol &result) const {
    for (; iIndex < GetCount(); iIndex++) {
        Symbol s = GetSymbol(iIndex);
        if (strcmp(type, s.type) == 0) {
            result = s;
            iIndex++;
            return true;
        }
    }
    return false;
}

} // namespace EAGL
