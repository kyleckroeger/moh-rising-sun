// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
void TARExtension::Use(GXTexMapID unit, unsigned int paletteId) {
    SetTextureLODStates();
    if (usesPalette) {
        GXLoadTlut(&palette, paletteId);
        GXInitTexObjTlut(&texture, paletteId);
    }
    GXLoadTexObj(&texture, unit);
    const SHAPE *clut = SHAPE_clut(shape);
    if (clut && ShapeHeader(clut).format == 51) {
        GXTlutObj second;
        const void *pixels = ShapePixels(clut);
        unsigned short count = ShapeHeader(clut).width;
        void *data = (void *)(((unsigned int)pixels + ((unsigned int)count << 1) + 31) & ~31);
        GXInitTlutObj(&second, data, GX_TL_IA8, count);
        GXLoadTlut(&second, ++paletteId);
        GXInitTexObjTlut(&texture, paletteId);
        GXLoadTexObj(&texture, (GXTexMapID)(unit + 1));
    }
}
