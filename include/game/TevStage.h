#ifndef GAME_EAGL_TEV_STAGE_H
#define GAME_EAGL_TEV_STAGE_H
// Reconstructed from the GR8E69 setters, GX consumers and array-copy stride.
// Member names below are descriptive, not recovered original field names.
// See docs/Rendering.md for layout evidence and unresolved source details.
#pragma interface
#include <dolphin/gx/GXEnum.h>
namespace EAGL {
class TevStage {
public:
    GXTevAlphaArg alpha[4];
    GXTevColorArg colour[4];
    GXTevOp alphaOp;
    GXTevBias alphaBias;
    GXTevScale alphaScale;
    unsigned char alphaClamp;
    GXTevRegID alphaOut;
    GXTevOp colourOp;
    GXTevBias colourBias;
    GXTevScale colourScale;
    unsigned char colourClamp;
    GXTevRegID colourOut;
    GXTexMapID texture;
    GXTexCoordID coords;
    GXChannelID channel;
    GXTevKColorSel kColour;
    GXTevKAlphaSel kAlpha;
    static unsigned char gNumActiveStages;
    static bool gTevStageDirtyFlag;
    static bool gPS2StyleOverBrightColours;
    static bool gPS2StyleOverBrightAlpha;
    TevStage();
    static void SetHardware();
    static void ResetToDefault();
    void SetAlphaIn(GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg);
    void SetColourIn(GXTevColorArg, GXTevColorArg, GXTevColorArg, GXTevColorArg);
    void SetAlphaOp(GXTevOp, GXTevBias, GXTevScale, unsigned char, GXTevRegID);
    void SetColourOp(GXTevOp, GXTevBias, GXTevScale, unsigned char, GXTevRegID);
    void SetKAlphaSel(GXTevKAlphaSel);
    void SetTextureMapAndUVs(GXTexMapID, GXTexCoordID);
    void SetColourSource(GXChannelID);
    void Use(unsigned int) const;
    static void SetPS2StyleOverBright(bool, bool);
    static void SetNumActiveStages(unsigned char);
};
class TevStages {
public:
    static TevStage gTevStages[16];
};


}
#endif
