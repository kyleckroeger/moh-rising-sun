// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
static inline void DefaultFormatCase() {}
int TARExtension::InitFromShape(const SHAPE *input) {
    if (input)
        shape = input;
    else
        shape = EAGLInternal::GetFailedShape();
    numMipmaps = ((ShapeHeader(shape).flags >> 12) & 15) + 1;
    if (numMipmaps > 1 && ((ShapeHeader(shape).width & (ShapeHeader(shape).width - 1)) ||
                           (ShapeHeader(shape).height & (ShapeHeader(shape).height - 1))))
        numMipmaps = 1;
    usesPalette = false;
    switch ((unsigned int)ShapeHeader(shape).format) {
    case 16:
        format = GX_TF_I4;
        break;
    case 17:
        format = GX_TF_I8;
        break;
    case 18:
        format = GX_TF_IA4;
        break;
    case 19:
        format = GX_TF_IA8;
        break;
    case 20:
        format = GX_TF_RGB565;
        break;
    case 21:
        format = GX_TF_RGB5A3;
        break;
    case 22:
        format = GX_TF_RGBA8;
        break;
    case 24:
        format = (GXTexFmt)GX_TF_C4;
        usesPalette = true;
        break;
    case 25:
        format = (GXTexFmt)GX_TF_C8;
        usesPalette = true;
        break;
    case 26:
        format = (GXTexFmt)GX_TF_C14X2;
        usesPalette = true;
        break;
    case 30:
        DefaultFormatCase();
        format = GX_TF_CMPR;
        break;
    default:
        format = GX_TF_CMPR;
        break;
    }
    if (usesPalette) {
        if (!SetClut(SHAPE_clut(shape))) {
            PrintMessage(0, "EAGL::TARExtension::InitFromShape - Invalid CLUT type in shape");
            return 0;
        }
        GXInitTexObjCI(&texture, (void *)ShapePixels(shape), ShapeHeader(shape).width,
                       ShapeHeader(shape).height, (GXCITexFmt)format, clampU, clampV, GX_FALSE, 0);
    } else
        GXInitTexObj(&texture, (void *)ShapePixels(shape), ShapeHeader(shape).width,
                     ShapeHeader(shape).height, format, clampU, clampV, GX_FALSE);
    return 1;
}
