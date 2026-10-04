#include "EAGLPropertyParser.h"
// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
#include <stdlib.h>
#include <stdio.h>
namespace EAGLInternal {
static inline int ParseTarEnum(const char *text) {
    if (strncmp(text, "EAGL::CM_", 9) == 0) {
        if (strcmp(text, "EAGL::CM_CLAMP") == 0)
            return 0;
        if (strcmp(text, "EAGL::CM_WRAP") == 0)
            return 1;
        if (strcmp(text, "EAGL::CM_MIRROR") == 0)
            return 2;
    } else if (strncmp(text, "EAGL::FM_", 9) == 0) {
        if (strcmp(text, "EAGL::FM_POINT") == 0)
            return 1;
        if (strcmp(text, "EAGL::FM_BILINEAR") == 0)
            return 2;
        if (strcmp(text, "EAGL::FM_ANISOTROPIC") == 0)
            return 3;
        if (strcmp(text, "EAGL::FM_QUINCUNX") == 0)
            return -1;
        if (strcmp(text, "EAGL::FM_GAUSSIANCUBIC") == 0)
            return -1;
    } else if (strncmp(text, "EAGL::MMM_", 10) == 0) {
        if (strcmp(text, "EAGL::MMM_OFF") == 0)
            return 0;
        if (strcmp(text, "EAGL::MMM_NEAREST") == 0)
            return 1;
        if (strcmp(text, "EAGL::MMM_LINEAR") == 0)
            return 2;
    }
    EAGL::PrintMessage(0, "INTERNAL ERROR: Invalid TAR api function parameter '%s'\n", text);
    return 0;
}
static inline bool ParseBool(const char *s) { return strcmp(s, "true") == 0; }
static inline unsigned int ParseHex(const char *s) {
    unsigned int value;
    sscanf(s, "%x", &value);
    return value;
}
static inline void HandleTarProp(EAGL::TAR &tar, const Property &p) {
    if (p.Matches("SetClampMode", 1))
        tar.SetClampMode((EAGL::ClampMode)ParseTarEnum(p.arguments[0]));
    else if (p.Matches("SetFilterMode", 1))
        tar.SetFilterMode((EAGL::FilterMode)ParseTarEnum(p.arguments[0]));
    else if (p.Matches("SetMIPMAPLODBias", 1))
        tar.SetMIPMAPLODBias(atof(p.arguments[0]));
    else if (p.Matches("SetMIPMAPMode", 1))
        tar.SetMIPMAPMode((EAGL::MIPMAPMode)ParseTarEnum(p.arguments[0]));
}
static inline EAGL::GCClampMode ParseGCEnum(const char *text) {
    if (strncmp(text, "EAGL::GCCM_", 11) == 0) {
        if (strcmp(text, "EAGL::GCCM_CLAMP") == 0)
            return EAGL::GCCM_CLAMP;
        if (strcmp(text, "EAGL::GCCM_WRAP") == 0)
            return EAGL::GCCM_WRAP;
        if (strcmp(text, "EAGL::GCCM_MIRROR") == 0)
            return EAGL::GCCM_MIRROR;
    }
    EAGL::PrintMessage(0, "INTERNAL ERROR: Invalid TAR value %s\n", text);
    return EAGL::GCCM_CLAMP;
}
void *RuntimeAllocTARConstructor(const char *source, EAGL::DynamicLoader *loader, int &arrayCount, bool &uid,
                                 const char *) {
    PropertyParser parser(source);
    int count = 1;
    EAGL::TAR *tar = 0;
    for (int i = 0; i < parser.GetCount(); ++i) {
        const Property &p = parser.GetProperty(i);
        if (p.Matches("UID"))
            uid = true;
        else if (p.Matches("SHAPENAME")) {
            if (!tar) {
                const char *name = p.arguments[0];
                count = atoi(p.arguments[1]);
                char symbol[12] = "shape_\0\0\0\0\0";
                symbol[6] = name[0];
                symbol[7] = name[1];
                symbol[8] = name[2];
                symbol[9] = name[3];
                symbol[10] = 0;
                bool found;
                const SHAPE *shape = (const SHAPE *)EAGL::DynamicLoader::gSymbolPool.Search(symbol, found);
                if (!found) {
                    void *address;
                    loader->GetAddr(EAGL::DynamicLoader::ShapeType, symbol, address);
                    shape = (const SHAPE *)address;
                }
                if (!shape)
                    shape = (const SHAPE *)(GetFailedShape());
                tar = (EAGL::TAR *)EAGLMalloc(count * ((sizeof(EAGL::TAR) + 31) & ~31), 0);
                new (tar) EAGL::TAR(shape);
                tar->GetShape();
            }
        } else if (tar) {
            if (p.Matches("CLUTNAME", 1)) {
                const char *name = p.arguments[0];
                const SHAPE *shape = 0;
                char symbol[12] = "shape_\0\0\0\0\0";
                symbol[6] = name[0];
                symbol[7] = name[1];
                symbol[8] = name[2];
                symbol[9] = name[3];
                symbol[10] = 0;
                bool found;
                shape = (const SHAPE *)EAGL::DynamicLoader::gSymbolPool.Search(symbol, found);
                if (!found) {
                    void *address;
                    shape = 0;
                    loader->GetAddr(EAGL::DynamicLoader::ShapeType, symbol, address);
                }
                if (shape) {
                    unsigned char format = *(const unsigned char *)shape;
                    if (format < 48 || format > 50)
                        shape = SHAPE_clut(shape);
                    if (shape)
                        tar->SwapClut(shape);
                }
            } else if (p.StartsWith("GCEXTOBJ")) {
                if (p.Matches("GCEXTOBJ_SetLODBiasClamp", 1))
                    tar->extension.SetLODBiasClamp(ParseBool(p.arguments[0]));
                else if (p.Matches("GCEXTOBJ_SetClampModeU", 1))
                    tar->extension.SetClampModeU(ParseGCEnum(p.arguments[0]));
                else if (p.Matches("GCEXTOBJ_SetClampModeV", 1))
                    tar->extension.SetClampModeV(ParseGCEnum(p.arguments[0]));
            } else
                HandleTarProp(*tar, p);
        }
    }
    for (int i = 1; i < count; ++i)
        new ((char *)tar + i * ((sizeof(EAGL::TAR) + 31) & ~31)) EAGL::TAR(*tar);
    arrayCount = count;
    return tar;
}
} // namespace EAGLInternal
