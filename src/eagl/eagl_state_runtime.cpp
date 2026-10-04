// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
#include "EAGLPropertyParser.h"
#include <stdlib.h>
#include <stdio.h>
namespace EAGL {
class DynamicLoader;
}
namespace EAGLInternal {
static inline bool ParseBool(const char *s) { return strcmp(s, "true") == 0; }
static inline EAGL::Colour ParseColour(const char *s) {
    EAGL::Colour c;
    sscanf(s, "0x%x", &c.packed);
    return c;
}
int StringToCommonStateEnum(const char *);
int StringToPlatformStateEnum(const char *);
void HandleDefaultStateProp(EAGL::GeoPrimState &, const Property &);
void *RuntimeAllocGeoPrimStateConstructor(const char *source, EAGL::DynamicLoader *, int &, bool &uid,
                                          const char *) {
    PropertyParser parser(source);
    EAGL::GeoPrimState *state = new EAGL::GeoPrimState;
    for (int i = 0; i < parser.GetCount(); ++i) {
        const Property &p = parser.GetProperty(i);
        if (p.Matches("UID"))
            uid = true;
        else if (p.StartsWith("GCEXTOBJ")) {
            if (p.Matches("GCEXTOBJ_SetZWritesEnable", 1))
                state->extension.SetZWritesEnable(ParseBool(p.arguments[0]));
            else if (p.Matches("GCEXTOBJ_SetCullDirection", 1))
                state->extension.SetCullDirection(
                    (EAGL::CullDirection)StringToCommonStateEnum(p.arguments[0]));
            else if (p.Matches("GCEXTOBJ_SetBlendMode", 4)) {
                const PropertyArgument *args = p.arguments;
                GXBlendMode mode = (GXBlendMode)StringToPlatformStateEnum(args[0]);
                GXBlendFactor source = (GXBlendFactor)StringToPlatformStateEnum(args[1]);
                GXBlendFactor dest = (GXBlendFactor)StringToPlatformStateEnum(args[2]);
                GXLogicOp op = (GXLogicOp)StringToPlatformStateEnum(args[3]);
                state->extension.SetBlendMode(mode, source, dest, op);
            } else if (p.Matches("GCEXTOBJ_SetCurrentVertex", 2)) {
                const PropertyArgument *args = p.arguments;
                int format = atoi(args[1]);
                int type = atoi(args[2]);
                state->extension.SetCurrentVertex(format, type);
            } else if (p.Matches("GCEXTOBJ_SetAttributeFormat", 4)) {
                const PropertyArgument *args = p.arguments;
                int format = atoi(args[0]);
                GXAttr attr = (GXAttr)StringToPlatformStateEnum(args[1]);
                GXCompCnt count = (GXCompCnt)StringToPlatformStateEnum(args[2]);
                GXCompType type = (GXCompType)StringToPlatformStateEnum(args[3]);
                unsigned char fraction = atoi(args[0]);
                state->extension.SetAttributeFormat(format, attr, count, type, fraction);
            }
        } else
            HandleDefaultStateProp(*state, p);
    }
    return state;
}
} // namespace EAGLInternal
