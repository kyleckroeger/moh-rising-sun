// Class-body include for leaf animation allocation; intentionally has no guard.
// Leaf deleting destructors call the original EAGL free callback. The original
// FnAnimMemoryMap destructor uses ordinary deletion, so this cannot live on the
// root class. Historical macro/header organization remains unknown.
#pragma interface
static void operator delete(void *ptr, unsigned int size) { EAGLInternal::EAGLFree(ptr, size); }
