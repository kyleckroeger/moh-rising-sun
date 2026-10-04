// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLMaterial.h"
using namespace EAGL;
using namespace EAGLInternal;
class GeoPrim_SingleDrawGouraud {
public:
    static int gNumVolatiles;
    static unsigned char gDefaultPCode[9];
    static unsigned int gIdentifier;
    static void* gLastVolatileSource[2];
    static void* gVolatileData[2];
    static int gNoUploadsConstantSize;
    static int gNoUploadsVariableSize;
};
extern StaticData materialState __asm__("staticdata.1227");
extern void CompBlock000SingleDrawGouraud(PCodeData&, StaticData&);
static void RenderSingleDrawGouraud(GeoPrim* primitive) {
    PCodeData data;
    data.identifier = &GeoPrim_SingleDrawGouraud::gIdentifier;
    data.defaultPCode = GeoPrim_SingleDrawGouraud::gDefaultPCode;
    data.numVolatiles = GeoPrim_SingleDrawGouraud::gNumVolatiles;
    data.lastVolatileSource = GeoPrim_SingleDrawGouraud::gLastVolatileSource;
    data.volatileData = GeoPrim_SingleDrawGouraud::gVolatileData;
    data.noUploadsConstantSize = &GeoPrim_SingleDrawGouraud::gNoUploadsConstantSize;
    data.noUploadsVariableSize = &GeoPrim_SingleDrawGouraud::gNoUploadsVariableSize;
    ProcessPCode(data, primitive, -1);
    if (data.resetState) {
        GXColor white = {255};
        white.g = 255;white.b = 255;white.a = 255;
        GXSetChanAmbColor(GX_COLOR0A0, white);
        GXSetChanMatColor(GX_COLOR0A0, white);
        TevStage::ResetToDefault();
        TARExtension::ResetTexCoordGens();
    }
    if (data.formatDirty || data.positionIndex8!=materialState.positionIndex8 || data.colourIndex8!=materialState.colourIndex8) {
        data.fastDisplayList = false;
        GXClearVtxDesc();
        materialState.positionIndex8 = data.positionIndex8;
        if (data.positionIndex8) GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        materialState.colourIndex8 = data.colourIndex8;
        if (data.colourIndex8) GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    }
    if (data.formatDirty) {
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        TARExtension::SetNumTexGens(0);
        GXSetNumChans(1);
        materialState.attributesPerVertex = 2;
        materialState.vertexFormat = 0x1200;
        materialState.preserveVertexFormat = 1;
        GeoPrimStateExtension::SetCurrentVertex(0x1200, -1);
        materialState.vertexColours = 1;
    }
    RenderMethod::SetArray(GX_VA_POS, GeoPrim_SingleDrawGouraud::gVolatileData[0], 12);
    RenderMethod::SetArray(GX_VA_CLR0, GeoPrim_SingleDrawGouraud::gVolatileData[1], 4);
    CompBlock000SingleDrawGouraud(data, materialState);
    if (data.variationCode) {
        while (data.variationCode[0]==2) {
            data.parameters[data.variationCode[1]].value = (char*)data.parameters[data.variationCode[1]].value - *(int*)(data.variationCode+2)*CurrentVariation;
            data.variationCode+=6;
        }
    }
}
