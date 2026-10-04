#pragma implementation "TevStage.h"
#include "TevStage.h"
#include "TevStageInlines.h"
#include <dolphin/gx/GXTev.h>
namespace EAGL {
unsigned char TevStage::gNumActiveStages = 1;
bool TevStage::gTevStageDirtyFlag = true;
bool TevStage::gPS2StyleOverBrightColours = false;
bool TevStage::gPS2StyleOverBrightAlpha = false;
// The original named array occupies initialized .data, with dynamic construction.
TevStage TevStages::gTevStages[16] __attribute__((section(".data")));

void TevStage::SetHardware() {
    if (!gTevStageDirtyFlag) return;
    int count = gNumActiveStages;
    gTevStageDirtyFlag = false;
    GXSetNumTevStages(count);
    for (int i = 0;i<count;++i) {
        TevStage &s = TevStages::gTevStages[i];
        GXTevScale cscale = s.colourScale, ascale = s.alphaScale;
        if (i==count-1) {
            if (gPS2StyleOverBrightColours) {
                if (cscale==GX_CS_SCALE_1) cscale = GX_CS_SCALE_2;
                else if (cscale==GX_CS_SCALE_2) cscale = GX_CS_SCALE_4;
            }
            if (gPS2StyleOverBrightAlpha) {
                if (s.alphaScale==GX_CS_SCALE_1) ascale = GX_CS_SCALE_2;
                else if (s.alphaScale==GX_CS_SCALE_2) ascale = GX_CS_SCALE_4;
            }
        }
        GXTevStageID stage = (GXTevStageID)i;
        GXSetTevOrder(stage, s.coords, (GXTexMapID)((unsigned)s.texture&0x7fffffff), s.channel);
        GXSetTevAlphaIn(stage, s.alpha[0], s.alpha[1], s.alpha[2], s.alpha[3]);
        GXSetTevAlphaOp(stage, s.alphaOp, s.alphaBias, ascale, s.alphaClamp, s.alphaOut);
        GXSetTevColorIn(stage, s.colour[0], s.colour[1], s.colour[2], s.colour[3]);
        GXSetTevColorOp(stage, s.colourOp, s.colourBias, cscale, s.colourClamp, s.colourOut);
        GXSetTevKColorSel(stage, s.kColour);
        GXSetTevKAlphaSel(stage, s.kAlpha);
    }
}
void TevStage::ResetToDefault() {
    TevStage def;
    TevStages::gTevStages[0] = def;
    for (int i = 1;i<16;++i) {
        TevStages::gTevStages[i] = def;
        TevStages::gTevStages[i].SetTextureMapAndUVs((GXTexMapID)i, (GXTexCoordID)i);
        TevStages::gTevStages[i].SetColourIn(GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
        TevStages::gTevStages[i].SetAlphaIn(GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    }
    SetNumActiveStages(1);
}
}
