// AI-assisted Rising Sun adaptation of the CC0 nfsmw reference; see NOTICE and docs/Loading.md.
// Adapted from nfsmw pinned CC0 reference; older target verified separately.
#include "EAGLDynamicLoader.h"
namespace EAGL {
DynamicLoader::DynamicLoader(void *d, unsigned len, void *r, bool delayResolve, DynamicUserCallback cb)
    : mpData((char *)d), mDataLen(len), mpReloc((char *)r), mIsResolved(false), handle(0), nDestructors(0),
      destructors(0), RuntimeAllocDestructors(0), mSearchCallback(cb), mNumPatchAddresses(0),
      mMaxPatchAddresses(0), mpPatchAddresses32(0) {
    Initialize(cb);
    if (!delayResolve)
        Resolve();
    DoVersionCheck();
}
bool DynamicLoader::DoVersionCheck() { return true; }
DynamicLoader::~DynamicLoader() {
    Release();
    RunDestructors();
    if (mpPatchAddresses32) {
        EAGLInternal::EAGLFree(mpPatchAddresses32, mMaxPatchAddresses);
    }
    mpPatchAddresses32 = 0;
}

void DynamicLoader::Release() {
    if (handle) {
        HashPointer *h = reinterpret_cast<HashPointer *>(handle);

        if (h->prev) {
            h->prev->next = h->next;
        } else {
            hashhead = h->next;
        }

        if (h->next) {
            h->next->prev = h->prev;
        }

        if (h->chain) {
            EAGLInternal::EAGLFree(h->chain, h->symbols_num * sizeof(unsigned int));
        }

        if (h->isOriginal) {
            EAGLInternal::EAGLFree(h->isOriginal, h->symbols_num * sizeof(unsigned int));
        }

        // The enclosing handle check establishes a nonnull object here.
        h->~HashPointer();
        HashPointer::operator delete(h, sizeof(*h));
        handle = 0;
    }
    mIsResolved = false;
}

void DynamicLoader::RunDestructors() {
    if (destructors) {
        for (int i = nDestructors - 1; i >= 0; i--) {
            destructors[i].d(destructors[i].data);
        }

        EAGLInternal::EAGLFree(destructors, nDestructors * sizeof(*destructors));
        destructors = 0;
    }
    RuntimeAllocDestructorEntry *de = RuntimeAllocDestructors;
    while (de) {
        RuntimeAllocDestructorEntry *tempde = de->next;

        de->d(de->data, de->auxData);
        delete de;

        de = tempde;
    }

    RuntimeAllocDestructors = 0;
}

int DynamicLoader::GetCount() const {
    if (!handle) {
        return 0;
    }
    HashPointer *h = reinterpret_cast<HashPointer *>(handle);
    return h->symbols_num;
}

void *DynamicLoader::GetElfData() const { return mpData; }
} // namespace EAGL
