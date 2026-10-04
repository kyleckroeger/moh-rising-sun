#include "FlexProp.h"
extern "C" int strcmp(const char *, const char *);
bool FlexProp::IsA(const char *name) const {
    FlexPropClassView *d = format->description;
    while (d) {
        if (strcmp(d->name, name) == 0)
            return true;
        d = d->parent;
    }
    return false;
}
