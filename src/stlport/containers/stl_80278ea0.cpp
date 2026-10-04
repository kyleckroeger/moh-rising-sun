// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <deque>
class ThreadJob;

typedef _STL::_Deque_base<ThreadJob*, _STL::allocator<ThreadJob* > > TargetContainer;
template void TargetContainer::_M_create_nodes(ThreadJob***, ThreadJob***);
