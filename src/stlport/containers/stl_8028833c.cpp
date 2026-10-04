// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
#include <string>
class FlexPropFormat;

typedef _STL::_Rb_tree<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > >, _STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropFormat* >, _STL::_Select1st<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropFormat* > >, _STL::less<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > >, _STL::allocator<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropFormat* > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropFormat* > >*);
