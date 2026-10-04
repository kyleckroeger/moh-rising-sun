// AI-assisted reconstruction; evidence and scope: docs/GameAlgorithms.md.
// STLport templates retain their upstream notices.
#include "profile.h"
#include <map>
#include <vector>
#include "ResourceTypes.h"
typedef _STL::_Rb_tree<
    _STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> >,
    _STL::pair<_STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> > const,
               EALA::Resource::Pointer<EALA::Character::State> >,
    _STL::_Select1st<
        _STL::pair<_STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> > const,
                   EALA::Resource::Pointer<EALA::Character::State> > >,
    StringCompare,
    _STL::allocator<
        _STL::pair<_STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> > const,
                   EALA::Resource::Pointer<EALA::Character::State> > > >
    TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(
    _STL::_Rb_tree_node_base *, _STL::_Rb_tree_node_base *,
    _STL::pair<_STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> > const,
               EALA::Resource::Pointer<EALA::Character::State> > const &,
    _STL::_Rb_tree_node_base *);
