// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
static inline void DefaultFormatCase() {}
bool TARExtension::SetClut(const SHAPE *clut) {
    if (usesPalette && clut) {
        GXTlutFmt format;
        switch ((unsigned int)ShapeHeader(clut).format) {
        case 49:
            format = GX_TL_RGB565;
            break;
        case 50:
            format = GX_TL_RGB5A3;
            break;
        case 51:
            DefaultFormatCase();
            format = GX_TL_IA8;
            break;
        case 48:
        default:
            format = GX_TL_IA8;
            break;
        }
        unsigned short count = ShapeHeader(clut).width;
        void *pixels = (void *)ShapePixels(clut);
        GXInitTlutObj(&palette, pixels, format, count);
        return true;
    }
    return false;
}
