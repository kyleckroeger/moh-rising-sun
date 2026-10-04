// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
#include <string>
class FlexPropClassFormat;

typedef _STL::_Rb_tree<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > >, _STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropClassFormat* >, _STL::_Select1st<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropClassFormat* > >, _STL::less<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > >, _STL::allocator<_STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropClassFormat* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<_STL::basic_string<char, _STL::char_traits<char >, _STL::allocator<char > > const, FlexPropClassFormat* > const&, _STL::_Rb_tree_node_base*);
