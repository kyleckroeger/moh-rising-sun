// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLMaterial.h"
using namespace EAGL;
using namespace EAGLInternal;
class GeoPrim_Moh3_Msh_Texture2Envmap {
public:
    static int gNumVolatiles;
    static unsigned char gDefaultPCode[2];
    static unsigned int gIdentifier;
    static void* gLastVolatileSource;
    static void* gVolatileData;
    static int gNoUploadsConstantSize;
    static int gNoUploadsVariableSize;
};
extern StaticData materialState __asm__("staticdata.1302");
extern void CompBlock003Moh3_Msh_Texture2Envmap(PCodeData&, StaticData&);
extern void CompBlock004Moh3_Msh_Texture2Envmap(PCodeData&, StaticData&);
static inline void EmptyBlock(PCodeData&, StaticData&) {}
static void RenderMoh3_Msh_Texture2Envmap(GeoPrim* primitive) {
    PCodeData data;
    data.identifier = &GeoPrim_Moh3_Msh_Texture2Envmap::gIdentifier;
    data.defaultPCode = GeoPrim_Moh3_Msh_Texture2Envmap::gDefaultPCode;
    data.numVolatiles = GeoPrim_Moh3_Msh_Texture2Envmap::gNumVolatiles;
    data.lastVolatileSource = &GeoPrim_Moh3_Msh_Texture2Envmap::gLastVolatileSource;
    data.volatileData = &GeoPrim_Moh3_Msh_Texture2Envmap::gVolatileData;
    data.noUploadsConstantSize = &GeoPrim_Moh3_Msh_Texture2Envmap::gNoUploadsConstantSize;
    data.noUploadsVariableSize = &GeoPrim_Moh3_Msh_Texture2Envmap::gNoUploadsVariableSize;
    ProcessPCode(data, primitive, 11);
    if (data.resetState) {
        GXColor white = {255};
        white.g = 255;white.b = 255;white.a = 255;
        GXSetChanAmbColor(GX_COLOR0A0, white);
        GXSetChanMatColor(GX_COLOR0A0, white);
        TevStage::ResetToDefault();
        TARExtension::ResetTexCoordGens();
    }
    if (data.formatDirty || data.positionIndex8!=materialState.positionIndex8 || data.colourIndex8!=materialState.colourIndex8 || data.normalIndex8!=materialState.normalIndex8 || data.textureIndex8[0]!=materialState.textureIndex8[0]) {
        data.fastDisplayList = false;
        GXClearVtxDesc();
        materialState.positionIndex8 = data.positionIndex8;
        if (data.positionIndex8) GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        materialState.colourIndex8 = data.colourIndex8;
        if (data.colourIndex8) GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        materialState.normalIndex8 = data.normalIndex8;
        if (data.normalIndex8) GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        materialState.textureIndex8[0] = data.textureIndex8[0];
        if (data.textureIndex8[0]) GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    }
    if (data.formatDirty) {
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 10);
        TARExtension::SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0, GX_PTIDENTITY);
        TARExtension::SetNumTexGens(1);
        GXSetNumChans(1);
        materialState.attributesPerVertex = 4;
        materialState.vertexFormat = 0x5600;
        materialState.preserveVertexFormat = 1;
        GeoPrimStateExtension::SetCurrentVertex(0x5600, -1);
        materialState.vertexColours = 1;
    }
    RenderMethod::SetArray(GX_VA_POS, data.parameters[4].value, 6);
    RenderMethod::SetArray(GX_VA_CLR0, data.parameters[5].value, 4);
    RenderMethod::SetArray(GX_VA_NRM, data.parameters[6].value, 3);
    RenderMethod::SetArray(GX_VA_TEX0, data.parameters[7].value, 4);
    switch(data.block) {
    case 0: EmptyBlock(data, materialState);break;
    case 1: EmptyBlock(data, materialState);break;
    case 2: EmptyBlock(data, materialState);break;
    case 3: CompBlock003Moh3_Msh_Texture2Envmap(data, materialState);break;
    case 4: CompBlock004Moh3_Msh_Texture2Envmap(data, materialState);break;
    }
    if (data.variationCode) {
        while (data.variationCode[0]==2) {
            data.parameters[data.variationCode[1]].value = (char*)data.parameters[data.variationCode[1]].value - *(int*)(data.variationCode+2)*CurrentVariation;
            data.variationCode+=6;
        }
    }
}
