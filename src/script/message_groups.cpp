#include "ScriptRuntime.h"
int BSMessageIsGroupMember(TriggerObject *object, unsigned int match) {
    if (!object) return 0;
    unsigned int group = match >> 16;
    FlexPropList *list = object->GetList("Group");
    if (list) {
        int count = list->count;
        for (int i = 0; i < count; ++i)
            if ((list->values[i] & 255) == group) return 1;
    }
    return 0;
}
int BSMessageAreInSameGroup(BSObject *first, BSObject *second) {
    if (!first || !second) return 0;
    FlexPropList *a = first->nativeObject->GetList("Group");
    FlexPropList *b = second->nativeObject->GetList("Group");
    if (a && b) {
        int ac = a->count;
        int bc = b->count;
        for (int i = 0; i < ac; ++i) {
            int *value = &a->values[i];
            for (int j = 0; j < bc; ++j)
                if (*value == b->values[j]) return 1;
        }
    }
    return 0;
}
int BSMessageIsGroupCategoryMember(TriggerObject *first, TriggerObject *second, unsigned int match) {
    if (!first || !second) return 0;
    FlexPropList *a = first->GetList("Group");
    FlexPropList *b = second->GetList("Group");
    if (a && b) {
        int ac = a->count;
        int bc = b->count;
        unsigned int category = match >> 16;
        for (int i = 0; i < ac; ++i) {
            int av = a->values[i];
            if ((av >> 16) == category) {
                for (int j = 0; j < bc; ++j) {
                    int bv = b->values[j];
                    if ((bv >> 16) == category && av == bv) return 1;
                }
            }
        }
    }
    return 0;
}
