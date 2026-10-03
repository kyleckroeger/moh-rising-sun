// AI-assisted reconstruction of the allocation-operator fragment of sys_memory.cpp.
// Names/ranges and labels are supported by the target; parameter names are descriptive.
// See docs/Memory.md. DWI_alloc/DWI_free and g_memLabel remain original context.
extern "C" {
extern const char* g_memLabel;
void* DWI_alloc(const char* label, int size, int flags);
int DWI_free(void* pointer);
}

void* operator new(unsigned int size)
{
    return DWI_alloc(g_memLabel ? g_memLabel : "operator new", size, 1024);
}

void* operator new[](unsigned int size)
{
    return DWI_alloc(g_memLabel ? g_memLabel : "operator new[]", size, 1024);
}

void operator delete(void* pointer)
{
    DWI_free(pointer);
}

void operator delete[](void* pointer)
{
    DWI_free(pointer);
}
