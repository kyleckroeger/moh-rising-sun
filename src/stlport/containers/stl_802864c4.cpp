// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <algorithm>
#include <list>
class CCoverPoint;
namespace _STL {
typedef _List_iterator<CCoverPoint *, _Nonconst_traits<CCoverPoint *> > CoverIterator;
template CoverIterator find<CoverIterator, CCoverPoint *>(CoverIterator, CoverIterator,
                                                          CCoverPoint *const &);
} // namespace _STL
