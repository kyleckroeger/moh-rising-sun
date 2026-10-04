// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
MemoryPoolManager::~MemoryPoolManager() {}

unsigned int MemoryPoolManager::GetMemoryPoolUsageAux() { return gMemoryPoolFree - gMemoryPool; }

unsigned int MemoryPoolManager::GetFreePoolSizeAux() { return gMemoryPoolSize - GetMemoryPoolUsage(); }

} // namespace EAGLAnim
