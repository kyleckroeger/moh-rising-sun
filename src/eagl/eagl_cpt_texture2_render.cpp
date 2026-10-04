// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLMaterial.h"
using namespace EAGL;
using namespace EAGLInternal;
class GeoPrim_Moh3_Cpt_Texture2 {
public:
    static int gNumVolatiles;
    static unsigned char gDefaultPCode[2];
    static unsigned int gIdentifier;
    static void* gLastVolatileSource;
    static void* gVolatileData;
    static int gNoUploadsConstantSize;
    static int gNoUploadsVariableSize;
};
extern StaticData materialState __asm__("staticdata.1242");
extern void CompBlock003Moh3_Cpt_Texture2(PCodeData&, StaticData&);
extern void CompBlock004Moh3_Cpt_Texture2(PCodeData&, StaticData&);
static inline void EmptyBlock(PCodeData&, StaticData&) {}
static void RenderMoh3_Cpt_Texture2(GeoPrim* primitive) {
    PCodeData data;
    data.identifier = &GeoPrim_Moh3_Cpt_Texture2::gIdentifier;
    data.defaultPCode = GeoPrim_Moh3_Cpt_Texture2::gDefaultPCode;
    data.numVolatiles = GeoPrim_Moh3_Cpt_Texture2::gNumVolatiles;
    data.lastVolatileSource = &GeoPrim_Moh3_Cpt_Texture2::gLastVolatileSource;
    data.volatileData = &GeoPrim_Moh3_Cpt_Texture2::gVolatileData;
    data.noUploadsConstantSize = &GeoPrim_Moh3_Cpt_Texture2::gNoUploadsConstantSize;
    data.noUploadsVariableSize = &GeoPrim_Moh3_Cpt_Texture2::gNoUploadsVariableSize;
    ProcessPCode(data, primitive, 17);
    if (data.resetState) {
        GXColor white = {255};
        white.g = 255;white.b = 255;white.a = 255;
        GXSetChanAmbColor(GX_COLOR0A0, white);
        GXSetChanMatColor(GX_COLOR0A0, white);
        TevStage::ResetToDefault();
        TARExtension::ResetTexCoordGens();
    }
    if (data.formatDirty || data.positionIndex8!=materialState.positionIndex8 || data.colourIndex8!=materialState.colourIndex8 || data.normalIndex8!=materialState.normalIndex8 || data.textureIndex8[0]!=materialState.textureIndex8[0] || data.textureIndex8[1]!=materialState.textureIndex8[1]) {
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
        materialState.textureIndex8[1] = data.textureIndex8[1];
        if (data.textureIndex8[1]) GXSetVtxDesc(GX_VA_TEX1, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_TEX1, GX_INDEX16);
    }
    if (data.formatDirty) {
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_POS, GX_POS_XYZ, GX_U16, 8);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 10);
        TARExtension::SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0, GX_PTIDENTITY);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_TEX1, GX_TEX_ST, GX_S16, 10);
        TARExtension::SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, 0, GX_PTIDENTITY);
        TARExtension::SetNumTexGens(2);
        GXSetNumChans(1);
        materialState.attributesPerVertex = 5;
        materialState.vertexFormat = 0xd600;
        materialState.preserveVertexFormat = 1;
        GeoPrimStateExtension::SetCurrentVertex(0xd600, -1);
        materialState.vertexColours = 1;
    }
    RenderMethod::SetArray(GX_VA_POS, data.parameters[4].value, 6);
    RenderMethod::SetArray(GX_VA_CLR0, data.parameters[5].value, 4);
    RenderMethod::SetArray(GX_VA_NRM, data.parameters[6].value, 3);
    RenderMethod::SetArray(GX_VA_TEX0, data.parameters[7].value, 4);
    RenderMethod::SetArray(GX_VA_TEX1, data.parameters[8].value, 4);
    switch(data.block) {
    case 0: EmptyBlock(data, materialState);break;
    case 1: EmptyBlock(data, materialState);break;
    case 2: EmptyBlock(data, materialState);break;
    case 5: break;
    case 3: CompBlock003Moh3_Cpt_Texture2(data, materialState);break;
    case 4: CompBlock004Moh3_Cpt_Texture2(data, materialState);break;
    }
    if (data.variationCode) {
        while (data.variationCode[0]==2) {
            data.parameters[data.variationCode[1]].value = (char*)data.parameters[data.variationCode[1]].value - *(int*)(data.variationCode+2)*CurrentVariation;
            data.variationCode+=6;
        }
    }
}
