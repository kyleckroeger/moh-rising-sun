#ifndef GAME_TEV_STAGE_INLINES_H
#define GAME_TEV_STAGE_INLINES_H
#include "TevStage.h"
namespace EAGL {
inline TevStage::TevStage()
{
    alpha[0] = GX_CA_ZERO;
    alpha[1] = GX_CA_TEXA;
    alpha[2] = GX_CA_RASA;
    alpha[3] = GX_CA_ZERO;
    colour[0] = GX_CC_ZERO;
    colour[1] = GX_CC_TEXC;
    colour[2] = GX_CC_RASC;
    colour[3] = GX_CC_ZERO;
    alphaOp = GX_TEV_ADD;
    alphaBias = GX_TB_ZERO;
    alphaScale = GX_CS_SCALE_1;
    alphaClamp = 1;
    alphaOut = GX_TEVPREV;
    colourOp = GX_TEV_ADD;
    colourBias = GX_TB_ZERO;
    colourScale = GX_CS_SCALE_1;
    colourClamp = 1;
    colourOut = GX_TEVPREV;
    texture = GX_TEXMAP0;
    coords = GX_TEXCOORD0;
    channel = GX_COLOR0A0;
    kColour = GX_TEV_KCSEL_K0;
    kAlpha = GX_TEV_KASEL_1;
}

inline void TevStage::SetAlphaIn(GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d)
{
    alpha[0] = a;
    alpha[1] = b;
    alpha[2] = c;
    alpha[3] = d;
    gTevStageDirtyFlag = true;
}

inline void TevStage::SetAlphaOp(GXTevOp op, GXTevBias bias, GXTevScale scale, unsigned char clamp, GXTevRegID out)
{
    alphaOp = op;
    alphaBias = bias;
    alphaScale = scale;
    alphaClamp = clamp;
    alphaOut = out;
    gTevStageDirtyFlag = true;
}

inline void TevStage::SetColourIn(GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d)
{
    colour[0] = a;
    colour[1] = b;
    colour[2] = c;
    colour[3] = d;
    gTevStageDirtyFlag = true;
}

inline void TevStage::SetColourOp(GXTevOp op, GXTevBias bias, GXTevScale scale, unsigned char clamp, GXTevRegID out)
{
    colourOp = op;
    colourBias = bias;
    colourScale = scale;
    colourClamp = clamp;
    colourOut = out;
    gTevStageDirtyFlag = true;
}

inline void TevStage::SetKAlphaSel(GXTevKAlphaSel sel)
{
    kAlpha = sel;
    gTevStageDirtyFlag = true;
}

inline void TevStage::SetTextureMapAndUVs(GXTexMapID tex, GXTexCoordID uv)
{
    texture = tex;
    coords = uv;
    gTevStageDirtyFlag = true;
}

inline void TevStage::SetColourSource(GXChannelID source)
{
    channel = source;
    gTevStageDirtyFlag = true;
}

inline void TevStage::Use(unsigned int stage) const
{
    if (&TevStages::gTevStages[stage] != this) {
        TevStages::gTevStages[stage] = *this;
        gTevStageDirtyFlag = true;
    }
}
inline void TevStage::SetPS2StyleOverBright(bool colour, bool alpha)
{
    if (gPS2StyleOverBrightColours != colour || gPS2StyleOverBrightAlpha != alpha)
        gTevStageDirtyFlag = true;
    gPS2StyleOverBrightColours = colour;
    gPS2StyleOverBrightAlpha = alpha;
}

inline void TevStage::SetNumActiveStages(unsigned char n)
{
    gNumActiveStages = n;
    gTevStageDirtyFlag = true;
}

}
#endif
