// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
#include "EAGLDynamicLoader.h"
namespace EAGL {
void DynamicLoader::RunConstructors() {
    int n = GetCount();
    int N = 0;

    for (int i = 0; i < n; i++) {
        Symbol s = GetSymbol(i);
        if (s.isInternalRef) {
            Constructor c = gConsPool.FindConstructor(s.type);
            if (c) {
                N++;
            }
        }
    }
    if (N > 0) {
        destructors = reinterpret_cast<DestructorEntry *>(
            EAGLInternal::EAGLMalloc(N * sizeof(*destructors), "EAGL::dynamic destructor list"));
        N = 0;
        for (int i = 0; i < n; i++) {
            Symbol s = GetSymbol(i);
            if (s.isInternalRef) {
                Constructor c = gConsPool.FindConstructor(s.type);
                if (!c) {
                    continue;
                }
                Destructor d = gConsPool.FindDestructor(s.type);
                c(s.data, this, s.name);
                destructors[N].d = d;
                destructors[N].data = s.data;
                N++;
            }
        }
    }
    nDestructors = N;
}

} // namespace EAGL
