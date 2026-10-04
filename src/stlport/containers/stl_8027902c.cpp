// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <deque>
class ThreadJob;
typedef _STL::_Deque_base<ThreadJob *, _STL::allocator<ThreadJob *> > JobDeque;
template JobDeque::~_Deque_base();
