// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLMaterial.h"
#include <dolphin/mtx.h>
using namespace EAGL;
using namespace EAGLInternal;
static void CompBlock003Moh3_Cpt_Texture1Envmap(PCodeData& data, StaticData& state) {
    state.state = (GeoPrimState*)data.parameters[8].value;
    float fog = ((float*)data.parameters[12].value)[0];
    bool fogEnabled;
    ContextExtension(Device::Get()->GetCurrentRenderContext())->GetFogEnable(fogEnabled);
    if (fog==1.0f && !fogEnabled)
        ContextExtension(Device::Get()->GetCurrentRenderContext())->SetFogEnable(true);
    else if (fog==0.0f && fogEnabled)
        ContextExtension(Device::Get()->GetCurrentRenderContext())->SetFogEnable(false);
    TARExtension::SetNumTexGens(2);
    TARExtension::SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX9, 0, GX_PTIDENTITY);
    TAR* environment = (TAR*)data.parameters[10].value;
    environment->SetClampMode(CLAMP_MODE_0);
    environment->SetFilterMode(FILTER_MODE_2);
    TevStage* stages = TevStages::gTevStages;
    stages[1].SetKAlphaSel(GX_TEV_KASEL_1);
    TevStage::SetNumActiveStages(3);
    stages[0].SetColourIn(GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    stages[1].SetColourOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    stages[1].SetColourIn(GX_CC_ZERO, GX_CC_TEXC, GX_CC_ONE, GX_CC_CPREV);
    stages[1].SetAlphaIn(GX_CA_ZERO, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
    stages[2].SetTextureMapAndUVs(GX_TEX_DISABLE, GX_TEXCOORD_NULL);
    stages[2].SetColourIn(GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    Transform view = *(Transform*)data.parameters[2].value;
    Transform transform;
    transform.BuildIdentity();
    float* position = (float*)data.parameters[11].value;
    transform.AppendTranslate(position[0], position[1], position[2]);
    transform.PostMult(view);
    Transform::Transpose(transform, transform);
    GXLoadPosMtxImm(transform.m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    {
    Transform normals = *(Transform*)data.parameters[1].value;
    Transform::Transpose(normals, normals);
    Mtx environmentMatrix;
    Mtx scale;
    Mtx translation;
    environmentMatrix[0][0] = normals.m[0][0];
    environmentMatrix[0][1] = normals.m[0][1];
    environmentMatrix[0][2] = normals.m[0][2];
    environmentMatrix[0][3] = 0.0f;
    environmentMatrix[1][0] = normals.m[1][0];
    environmentMatrix[1][1] = normals.m[1][1];
    environmentMatrix[1][2] = normals.m[1][2];
    environmentMatrix[1][3] = 0.0f;
    environmentMatrix[2][0] = 0.0f;
    environmentMatrix[2][1] = 0.0f;
    environmentMatrix[2][2] = 1.0f;
    environmentMatrix[2][3] = 0.0f;
    PSMTXScale(scale, 0.5f, -0.5f, 0.0f);
    PSMTXTrans(translation, 0.5f, 0.5f, 1.0f);
    PSMTXConcat(translation, scale, translation);
    PSMTXConcat(translation, environmentMatrix, environmentMatrix);
    GXLoadTexMtxImm(environmentMatrix, GX_TEXMTX9, GX_MTX2x4);
    }
    if (state.vertexColours) {
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    } else {
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    }
    TevStage::SetPS2StyleOverBright(true, false);
    ((GeoPrimStateExtension*)state.state)->Use();
    if (!state.preserveVertexFormat) GeoPrimStateExtension::SetCurrentVertex(state.vertexFormat, 3);
    if (data.textureDirty[0]) ((TAR*)data.parameters[9].value)->Use();
    Transform textureTransform = *(Transform*)data.parameters[14].value;
    Transform::Transpose(textureTransform, textureTransform);
    GXLoadTexMtxImm(textureTransform.m, GX_TEXMTX0, GX_MTX3x4);
    TARExtension::SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_TEXMTX0, 0, GX_PTIDENTITY);
    if (data.textureDirty[1])
        ((TARExtension*)((unsigned char*)data.parameters[10].value+4))->Use(GX_TEXMAP1, 1);
    TevStage::SetHardware();
    if (!data.drawArrays) {
        CallList(data.displayList, data.displayListSize, data.fastDisplayList);
    } else {
        PrimitiveType primitive;
        state.state->GetPrimitiveType(primitive);
        int count = *(int*)data.parameters[0].value;
        if (gpDisplayList) {
            int batch = (gDisplayListSize-3)/state.attributesPerVertex;
            batch = batch/12*12;
            GeoPrimStateExtension::SetCurrentVertex(state.vertexFormat, 2);
            if (*(int*)gpDisplayList==-1) {
                *gpDisplayList = (unsigned char)primitive;
                *(unsigned short*)(gpDisplayList+1) = batch;
                unsigned char* dst = gpDisplayList+3;
                for (unsigned char i = 0;i<batch;++i)
                    for (int j = 0;j<state.attributesPerVertex;++j) *dst++=i;
                while (dst<gpDisplayList+gDisplayListSize) *dst++=0;
                DCStoreRange(gpDisplayList, gDisplayListSize);
            }
            int bytes = (batch*state.attributesPerVertex+34)/32*32;
            bool called = false;
            while (count>=batch) {
                if (called) {
                    CallList(gpDisplayList, bytes, true);
                } else {
                    GXCallDisplayList(gpDisplayList, bytes);
                    called = true;
                }
                int consumed = batch;
                if (primitive==PRIMITIVE_TRIANGLE_STRIP) consumed-=2;
                RenderMethod::IncrementArrays(consumed);
                count-=consumed;
            }
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 0);
            FIFO8 = (unsigned char)primitive;
            FIFO16 = count;
            unsigned int i;
            int j;
            if (state.attributesPerVertex==4) {
                for (i = 0;i<(unsigned)count;++i) FIFO32 = i|(i<<8)|(i<<16)|(i<<24);
            } else if (state.attributesPerVertex==3) {
                for (i = 0;i<(unsigned)count;++i) {FIFO16 = i|(i<<8);FIFO8 = i;}
            } else if (state.attributesPerVertex==2) {
                for (i = 0;i<(unsigned)count;++i) FIFO16 = i|(i<<8);
            } else if (state.attributesPerVertex==1) {
                for (i = 0;i<(unsigned)count;++i) FIFO8 = i;
            } else {
                for (i = 0;i<(unsigned)count;++i)
                    for (j = 0;j<state.attributesPerVertex;++j) FIFO8 = i;
            }
            count*=state.attributesPerVertex;
            count+=3;
            while (count&31) {FIFO8 = 0;++count;}
        } else {
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 0);
            FIFO8 = (unsigned char)primitive;
            FIFO16 = count;
            unsigned int i;
            int j;
            if (state.attributesPerVertex==4) {
                for (i = 0;i<(unsigned)count;++i) {FIFO32 = i|(i<<16);FIFO32 = i|(i<<16);}
            } else if (state.attributesPerVertex==3) {
                for (i = 0;i<(unsigned)count;++i) {FIFO32 = i|(i<<16);FIFO16 = i;}
            } else if (state.attributesPerVertex==2) {
                for (i = 0;i<(unsigned)count;++i) FIFO32 = i|(i<<16);
            } else if (state.attributesPerVertex==1) {
                for (i = 0;i<(unsigned)count;++i) FIFO16 = i;
            } else {
                for (i = 0;i<(unsigned)count;++i)
                    for (j = 0;j<state.attributesPerVertex;++j) FIFO16 = i;
            }
            count+=count;
            count*=state.attributesPerVertex;
            count+=3;
            while (count&31) {FIFO8 = 0;++count;}
        }
    }
}
