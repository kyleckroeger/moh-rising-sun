// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
void GeoPrimStateExtension::SetCurrentVertex(unsigned int format, unsigned int type) {
    if (format != gVertexFormat || type != gVertexDataType) {
        if (type != (unsigned int)-1) {
            GXClearVtxDesc();
            if (format & (1u << 0))
                GXSetVtxDesc(GX_VA_PNMTXIDX, GX_DIRECT);
            if (format & (1u << 1))
                GXSetVtxDesc(GX_VA_TEX0MTXIDX, GX_DIRECT);
            if (format & (1u << 2))
                GXSetVtxDesc(GX_VA_TEX1MTXIDX, GX_DIRECT);
            if (format & (1u << 3))
                GXSetVtxDesc(GX_VA_TEX2MTXIDX, GX_DIRECT);
            if (format & (1u << 4))
                GXSetVtxDesc(GX_VA_TEX3MTXIDX, GX_DIRECT);
            if (format & (1u << 5))
                GXSetVtxDesc(GX_VA_TEX4MTXIDX, GX_DIRECT);
            if (format & (1u << 6))
                GXSetVtxDesc(GX_VA_TEX5MTXIDX, GX_DIRECT);
            if (format & (1u << 7))
                GXSetVtxDesc(GX_VA_TEX6MTXIDX, GX_DIRECT);
            if (format & (1u << 8))
                GXSetVtxDesc(GX_VA_TEX7MTXIDX, GX_DIRECT);
            if (format & (1u << 9))
                GXSetVtxDesc(GX_VA_POS, (GXAttrType)type);
            if (format & (1u << 10))
                GXSetVtxDesc(GX_VA_NRM, (GXAttrType)type);
            if (format & (1u << 11))
                GXSetVtxDesc(GX_VA_NBT, (GXAttrType)type);
            if (format & (1u << 12))
                GXSetVtxDesc(GX_VA_CLR0, (GXAttrType)type);
            if (format & (1u << 13))
                GXSetVtxDesc(GX_VA_CLR1, (GXAttrType)type);
            if (format & (1u << 14))
                GXSetVtxDesc(GX_VA_TEX0, (GXAttrType)type);
            if (format & (1u << 15))
                GXSetVtxDesc(GX_VA_TEX1, (GXAttrType)type);
            if (format & (1u << 16))
                GXSetVtxDesc(GX_VA_TEX2, (GXAttrType)type);
            if (format & (1u << 17))
                GXSetVtxDesc(GX_VA_TEX3, (GXAttrType)type);
            if (format & (1u << 18))
                GXSetVtxDesc(GX_VA_TEX4, (GXAttrType)type);
            if (format & (1u << 19))
                GXSetVtxDesc(GX_VA_TEX5, (GXAttrType)type);
            if (format & (1u << 20))
                GXSetVtxDesc(GX_VA_TEX6, (GXAttrType)type);
            if (format & (1u << 21))
                GXSetVtxDesc(GX_VA_TEX7, (GXAttrType)type);
        }
        gVertexFormat = format;
        gVertexDataType = type;
    }
}
