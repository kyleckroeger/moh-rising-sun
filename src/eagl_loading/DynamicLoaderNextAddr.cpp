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

inline bool DynamicLoader::GetNextSymbol(const char *type, int &iIndex, Symbol &result) const {
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

bool DynamicLoader::GetNextAddr(const char *type, int &iIndex, void *&addr) const {
    Symbol s;
    if (GetNextSymbol(type, iIndex, s)) {
        addr = s.data;
        return true;
    } else {
        return false;
    }
}

} // namespace EAGL
