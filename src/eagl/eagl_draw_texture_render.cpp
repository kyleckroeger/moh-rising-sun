// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLMaterial.h"
using namespace EAGL;
using namespace EAGLInternal;
class GeoPrim_SingleDrawTexture {
public:
    static int gNumVolatiles;
    static unsigned char gDefaultPCode[13];
    static unsigned int gIdentifier;
    static void* gLastVolatileSource[3];
    static void* gVolatileData[3];
    static int gNoUploadsConstantSize;
    static int gNoUploadsVariableSize;
};
extern StaticData materialState __asm__("staticdata.1227");
extern void CompBlock000SingleDrawTexture(PCodeData&, StaticData&);
static void RenderSingleDrawTexture(GeoPrim* primitive) {
    PCodeData data;
    data.identifier = &GeoPrim_SingleDrawTexture::gIdentifier;
    data.defaultPCode = GeoPrim_SingleDrawTexture::gDefaultPCode;
    data.numVolatiles = GeoPrim_SingleDrawTexture::gNumVolatiles;
    data.lastVolatileSource = GeoPrim_SingleDrawTexture::gLastVolatileSource;
    data.volatileData = GeoPrim_SingleDrawTexture::gVolatileData;
    data.noUploadsConstantSize = &GeoPrim_SingleDrawTexture::gNoUploadsConstantSize;
    data.noUploadsVariableSize = &GeoPrim_SingleDrawTexture::gNoUploadsVariableSize;
    ProcessPCode(data, primitive, -1);
    if (data.resetState) {
        GXColor white = {255};
        white.g = 255;white.b = 255;white.a = 255;
        GXSetChanAmbColor(GX_COLOR0A0, white);
        GXSetChanMatColor(GX_COLOR0A0, white);
        TevStage::ResetToDefault();
        TARExtension::ResetTexCoordGens();
    }
    if (data.formatDirty || data.positionIndex8!=materialState.positionIndex8 || data.colourIndex8!=materialState.colourIndex8 || data.textureIndex8[0]!=materialState.textureIndex8[0]) {
        data.fastDisplayList = false;
        GXClearVtxDesc();
        materialState.positionIndex8 = data.positionIndex8;
        if (data.positionIndex8) GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        materialState.colourIndex8 = data.colourIndex8;
        if (data.colourIndex8) GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        materialState.textureIndex8[0] = data.textureIndex8[0];
        if (data.textureIndex8[0]) GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    }
    if (data.formatDirty) {
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        TARExtension::SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0, GX_PTIDENTITY);
        TARExtension::SetNumTexGens(1);
        GXSetNumChans(1);
        materialState.attributesPerVertex = 3;
        materialState.vertexFormat = 0x5200;
        materialState.preserveVertexFormat = 1;
        GeoPrimStateExtension::SetCurrentVertex(0x5200, -1);
        materialState.vertexColours = 1;
    }
    RenderMethod::SetArray(GX_VA_POS, GeoPrim_SingleDrawTexture::gVolatileData[0], 12);
    RenderMethod::SetArray(GX_VA_CLR0, GeoPrim_SingleDrawTexture::gVolatileData[1], 4);
    RenderMethod::SetArray(GX_VA_TEX0, GeoPrim_SingleDrawTexture::gVolatileData[2], 8);
    CompBlock000SingleDrawTexture(data, materialState);
    if (data.variationCode) {
        while (data.variationCode[0]==2) {
            data.parameters[data.variationCode[1]].value = (char*)data.parameters[data.variationCode[1]].value - *(int*)(data.variationCode+2)*CurrentVariation;
            data.variationCode+=6;
        }
    }
}
