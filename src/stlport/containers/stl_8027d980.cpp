// Project reconstruction; scoped storage and behavior evidence: docs/GameContainers.md.
// AI-assisted integration. STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ContainerTypes.h"
typedef _STL::_Rb_tree<SHAPE*, _STL::pair<SHAPE* const, EAGL::Model::TARList >, _STL::_Select1st<_STL::pair<SHAPE* const, EAGL::Model::TARList > >, _STL::less<SHAPE* >, _STL::allocator<_STL::pair<SHAPE* const, EAGL::Model::TARList > > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(_STL::pair<SHAPE* const, EAGL::Model::TARList > const&);
