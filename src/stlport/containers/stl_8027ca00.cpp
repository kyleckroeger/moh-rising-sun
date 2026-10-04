// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ResourceTypes.h"
typedef _STL::_Rb_tree<
    EALA::Character::Skeleton::Partition const *,
    _STL::pair<EALA::Character::Skeleton::Partition const *const,
               EALA::Character::Choreographer::Context>,
    _STL::_Select1st<_STL::pair<EALA::Character::Skeleton::Partition const *const,
                                EALA::Character::Choreographer::Context> >,
    EALA::Character::Choreographer::PartitionCompare,
    _STL::allocator<_STL::pair<EALA::Character::Skeleton::Partition const *const,
                               EALA::Character::Choreographer::Context> > >
    TargetContainer;
template TargetContainer::iterator
TargetContainer::_M_insert(_STL::_Rb_tree_node_base *, _STL::_Rb_tree_node_base *,
                           _STL::pair<EALA::Character::Skeleton::Partition const *const,
                                      EALA::Character::Choreographer::Context> const &,
                           _STL::_Rb_tree_node_base *);
