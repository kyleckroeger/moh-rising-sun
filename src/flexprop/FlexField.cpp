#include "FlexProp.h"
#include <stdlib.h>
static int field_key_compare(const void *a, const void *b) {
    int x = *static_cast<const int *>(a), y = *static_cast<const int *>(b);
    if (x < y)
        return -1;
    return x > y;
}
FlexPropField *FlexPropFormat::GetField(int key) {
    FlexPropClassView *d = description;
    FlexPropField query; // Only the key is read by the comparator.
    query.crc = key;
    FlexPropField *result;
    do {
        result = static_cast<FlexPropField *>(bsearch(&query, d->fields, d->count, 12, field_key_compare));
        if (result)
            break;
        if (!d->parent)
            break;
        d = d->parent;
    } while (1);
    return result;
}
