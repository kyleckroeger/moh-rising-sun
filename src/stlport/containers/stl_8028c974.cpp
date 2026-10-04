// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class FEPackage;
class PackageHandler;

typedef _STL::_Rb_tree<FEPackage*, _STL::pair<FEPackage* const, PackageHandler* >, _STL::_Select1st<_STL::pair<FEPackage* const, PackageHandler* > >, _STL::less<FEPackage* >, _STL::allocator<_STL::pair<FEPackage* const, PackageHandler* > > > TargetContainer;
template void TargetContainer::_M_erase(_STL::_Rb_tree_node<_STL::pair<FEPackage* const, PackageHandler* > >*);
