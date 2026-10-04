// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class FEPackage;
class PackageHandler;

typedef _STL::_Rb_tree<FEPackage*, _STL::pair<FEPackage* const, PackageHandler* >, _STL::_Select1st<_STL::pair<FEPackage* const, PackageHandler* > >, _STL::less<FEPackage* >, _STL::allocator<_STL::pair<FEPackage* const, PackageHandler* > > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(_STL::pair<FEPackage* const, PackageHandler* > const&);
