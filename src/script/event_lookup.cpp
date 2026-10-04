// Scoped GameCube storage views for event lookup. Only array strides and fields
// accessed here are established; do not allocate complete classes from these views.
struct BSEventView {
    unsigned int unknown00;
    unsigned short eventNumber;
    unsigned char unknown06[2];
};

struct BSStateView {
    unsigned int unknown00;
    BSEventView *events;
    unsigned char unknown08[8];
    unsigned char eventCount;
};

struct BSLevelView {
    unsigned char unknown00[8];
    BSStateView **states;
    unsigned short stateCount;
    unsigned char unknown0e[14];
};

struct BSClass_struct {
    BSLevelView *levels;
    unsigned char unknown04[84];
    unsigned short levelCount;
};

int BSFindAnyEventHandler(unsigned short event, BSClass_struct *scriptClass) {
    for (int level = 0; level < scriptClass->levelCount; ++level) {
        for (int state = 0; state < scriptClass->levels[level].stateCount; ++state) {
            BSStateView *entry = scriptClass->levels[level].states[state];
            if (entry) {
                for (int handler = 0; handler < entry->eventCount; ++handler) {
                    if (entry->events[handler].eventNumber == event)
                        return 1;
                }
            }
        }
    }
    return 0;
}
