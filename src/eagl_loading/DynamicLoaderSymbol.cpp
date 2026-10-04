// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
#include "EAGLDynamicLoader.h"
namespace EAGL {
DynamicLoader::Symbol DynamicLoader::GetSymbol(int i) const {
    DynamicLoader::Symbol r;
    HashPointer *h = reinterpret_cast<HashPointer *>(handle);
    r.name = 0;
    r.data = 0;
    if (!h) {

        return r;
    }
    ELF32_Sym *s = h->symtab;
    if (i < 0 || i >= h->symbols_num) {

        return r;
    }
    r.name = &h->strtab[s[i].st_name];
    r.type = &h->strtab[s[i].st_name] + strlen(r.name);
    r.type++;
    if (r.type[0] == 0x7F) {
        r.type++;
    } else {
        r.type--;
    }
    r.isInternalRef = (unsigned int)(s[i].st_other - 2) > 3;

    int iIndex = s[i].st_shndx;
    if (s[i].st_other == 1) {
        r.data = reinterpret_cast<void *>(s[i].st_value);
    } else if (iIndex > 0 && iIndex < h->e->e_shnum) {
        r.data = reinterpret_cast<void *>(h->sections[iIndex].sh_offset + s[i].st_value);
    }

    return r;
}

} // namespace EAGL
