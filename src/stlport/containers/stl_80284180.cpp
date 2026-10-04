// Project explicit instantiation; original symbol types, opaque game pointees.
// AI-assisted integration; see docs/STLport.md.
#include "profile.h"
#include <map>
class MMGBidInfo;
class TriggerObject;

typedef _STL::_Rb_tree<TriggerObject*, _STL::pair<TriggerObject* const, MMGBidInfo* >, _STL::_Select1st<_STL::pair<TriggerObject* const, MMGBidInfo* > >, _STL::less<TriggerObject* >, _STL::allocator<_STL::pair<TriggerObject* const, MMGBidInfo* > > > TargetContainer;
template _STL::pair<TargetContainer::iterator, bool> TargetContainer::insert_unique(_STL::pair<TriggerObject* const, MMGBidInfo* > const&);
