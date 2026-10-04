// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
#include "EAGLDynamicLoader.h"
static void *dlsym(void *handle, const char *name) {
    EAGL::HashPointer *h = reinterpret_cast<EAGL::HashPointer *>(handle);
    EAGL::ELF32_Sym *s = h->symtab;
    unsigned long j = h->hash[EAGL::elfhash(name)];

    while (j != -1) {
        if (j > h->symbols_num)
            EAGL::PrintMessage(0, "dlsym: Internal error. Bad j value %d (chain size %d)!\n", j,
                               h->symbols_num);
        if (h->isOriginal[j] && strcmp(name, &h->strtab[s[j].st_name]) == 0) {
            int iIndex = s[j].st_shndx;
            if (iIndex > 0 && iIndex < h->e->e_shnum) {
                return reinterpret_cast<void *>(h->sections[iIndex].sh_offset + s[j].st_value);
            }
        }
        j = h->chain[j];
    }
    return 0;
}
