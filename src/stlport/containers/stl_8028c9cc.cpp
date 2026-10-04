// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class FEPackage;
class PackageHandler;

typedef _STL::_Rb_tree<FEPackage*, _STL::pair<FEPackage* const, PackageHandler* >, _STL::_Select1st<_STL::pair<FEPackage* const, PackageHandler* > >, _STL::less<FEPackage* >, _STL::allocator<_STL::pair<FEPackage* const, PackageHandler* > > > TargetContainer;
template TargetContainer::iterator TargetContainer::_M_insert(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*, _STL::pair<FEPackage* const, PackageHandler* > const&, _STL::_Rb_tree_node_base*);
