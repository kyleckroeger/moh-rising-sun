#ifndef GAME_EAGL_MEMORY_H
#define GAME_EAGL_MEMORY_H
// Original allocation callbacks remain externally owned data.
namespace EAGLInternal {
extern void *(*EAGLMalloc)(unsigned int, const char *);
extern void (*EAGLFree)(void *, unsigned int);
} // namespace EAGLInternal
#endif
