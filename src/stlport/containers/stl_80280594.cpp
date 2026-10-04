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
template TargetContainer::iterator TargetContainer::insert_unique(
    TargetContainer::iterator,
    _STL::pair<_STL::basic_string<char, _STL::char_traits<char>, _STL::allocator<char> > const,
               EALA::Resource::Pointer<EALA::Character::State> > const &);
