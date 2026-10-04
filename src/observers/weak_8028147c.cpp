// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "TargetTypes.h"
namespace EALA {
namespace Character {
class Task;
}
} // namespace EALA
template void WeakPtr<EALA::Character::Task, 8>::HandleEvent(ISubject *, ESubjectEvent);
