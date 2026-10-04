// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLMaterial.h"
using namespace EAGL;
using namespace EAGLInternal;
class GeoPrim_Particle {
public:
    static int gNumVolatiles;
    static unsigned char gDefaultPCode[4];
    static unsigned int gIdentifier;
    static void* gLastVolatileSource;
    static void* gVolatileData;
    static int gNoUploadsConstantSize;
    static int gNoUploadsVariableSize;
};
extern StaticData materialState __asm__("staticdata.1290");
extern void CompBlock000Particle(PCodeData&, StaticData&);
static void RenderParticle(GeoPrim* primitive) {
    PCodeData data;
    data.identifier = &GeoPrim_Particle::gIdentifier;
    data.defaultPCode = GeoPrim_Particle::gDefaultPCode;
    data.numVolatiles = GeoPrim_Particle::gNumVolatiles;
    data.lastVolatileSource = &GeoPrim_Particle::gLastVolatileSource;
    data.volatileData = &GeoPrim_Particle::gVolatileData;
    data.noUploadsConstantSize = &GeoPrim_Particle::gNoUploadsConstantSize;
    data.noUploadsVariableSize = &GeoPrim_Particle::gNoUploadsVariableSize;
    ProcessPCode(data, primitive, -1);
    if (data.resetState) {
        GXColor white = {255};
        white.g = 255;white.b = 255;white.a = 255;
        GXSetChanAmbColor(GX_COLOR0A0, white);
        GXSetChanMatColor(GX_COLOR0A0, white);
        TevStage::ResetToDefault();
        TARExtension::ResetTexCoordGens();
    }
    if (data.formatDirty || data.positionIndex8!=materialState.positionIndex8 || data.normalIndex8!=materialState.normalIndex8 || data.textureIndex8[0]!=materialState.textureIndex8[0]) {
        data.fastDisplayList = false;
        GXClearVtxDesc();
        materialState.positionIndex8 = data.positionIndex8;
        if (data.positionIndex8) GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        materialState.normalIndex8 = data.normalIndex8;
        if (data.normalIndex8) GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
        materialState.textureIndex8[0] = data.textureIndex8[0];
        if (data.textureIndex8[0]) GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
        else GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    }
    if (data.formatDirty) {
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 14);
        GeoPrimStateExtension::SetAttributeFormat(0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        TARExtension::SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0, GX_PTIDENTITY);
        TARExtension::SetNumTexGens(1);
        GXSetNumChans(0);
        materialState.vertexFormat = 0x4600;
        materialState.attributesPerVertex = 3;
        materialState.preserveVertexFormat = 1;
        GeoPrimStateExtension::SetCurrentVertex(0x4600, -1);
    }
    RenderMethod::SetArray(GX_VA_POS, data.parameters[3].value, 12);
    RenderMethod::SetArray(GX_VA_NRM, data.parameters[4].value, 6);
    RenderMethod::SetArray(GX_VA_TEX0, data.parameters[5].value, 8);
    CompBlock000Particle(data, materialState);
    if (data.variationCode) {
        while (data.variationCode[0]==2) {
            data.parameters[data.variationCode[1]].value = (char*)data.parameters[data.variationCode[1]].value - *(int*)(data.variationCode+2)*CurrentVariation;
            data.variationCode+=6;
        }
    }
}
