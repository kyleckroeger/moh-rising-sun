#include "EAGLMaterial.h"
// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
using namespace EAGL;
bool GeoPrimStateExtension::Use() const {
    bool result = true;
    AlphaWritesOverride override;
    if (!gStateInitialized) {
        gCurrentState = *this;
        unsigned char *p = (unsigned char *)&gCurrentState;
        unsigned char *end = p + sizeof(gCurrentState);
        while (p < end) {
            ++*p;
            ++p;
        }
        gStateInitialized = true;
    }
    bool clockwise = gCullClockwise;
    bool zWrites = gZWritesEnable;
    if (cullOverride)
        clockwise = cullOverride == 2;
    if (zWriteOverride)
        zWrites = zWriteOverride == 1;
    if (cull)
        GXSetCullMode(clockwise ? GX_CULL_FRONT : GX_CULL_BACK);
    else
        GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, (GXCompare)depthTest, zWrites ? GX_TRUE : GX_FALSE);
    int activeBlend = blend;
    if (transparency != gCurrentState.transparency) {
        switch (transparency) {
        case TM_OPAQUE:
            activeBlend = -1;
            break;
        case TM_ALPHA:
            gCurrentState.blend = (AlphaBlendMode)(activeBlend + 1);
            break;
        case TM_CHROMAKEY:
            result = false;
            break;
        }
    }
    if (activeBlend != gCurrentState.blend || activeBlend == ABM_CUSTOM) {
        if (transparency == TM_ALPHA || activeBlend == -1) {
            GXBlendMode mode;
            GXBlendFactor source, destination;
            GXLogicOp logic;
            if (activeBlend == -1) {
                mode = GX_BM_NONE;
                source = GX_BL_ONE;
                destination = GX_BL_ZERO;
                logic = GX_LO_COPY;
            } else {
                logic = GX_LO_COPY;
                switch (blend) {
                case ABM_OFF:
                    mode = GX_BM_NONE;
                    source = GX_BL_ONE;
                    destination = GX_BL_ZERO;
                    break;
                case ABM_BLEND:
                    mode = GX_BM_BLEND;
                    source = GX_BL_SRCALPHA;
                    destination = GX_BL_INVSRCALPHA;
                    break;
                case ABM_ADD:
                    mode = GX_BM_BLEND;
                    source = GX_BL_SRCALPHA;
                    destination = GX_BL_ONE;
                    break;
                case ABM_ATTENUATE:
                    mode = GX_BM_BLEND;
                    source = GX_BL_SRCALPHA;
                    destination = GX_BL_ZERO;
                    break;
                case ABM_MODULATE:
                    mode = GX_BM_BLEND;
                    source = GX_BL_SRCCLR;
                    destination = GX_BL_ZERO;
                    break;
                case ABM_SUBTRACT:
                    mode = GX_BM_SUBTRACT;
                    source = GX_BL_ONE;
                    destination = GX_BL_ONE;
                    break;
                case ABM_CUSTOM:
                    mode = customMode;
                    source = customSource;
                    destination = customDestination;
                    logic = customLogic;
                    break;
                }
            }
            GXSetBlendMode(mode, source, destination, logic);
            GXSetZCompLoc(GX_FALSE);
        }
    }
    if (alphaCompare != gCurrentState.alphaCompare && alphaTest)
        gCurrentState.alphaTest = false;
    if (alphaMethod != gCurrentState.alphaMethod && alphaTest)
        gCurrentState.alphaTest = false;
    if (alphaTest != gCurrentState.alphaTest) {
        if (alphaTest)
            GXSetAlphaCompare((GXCompare)alphaMethod, (unsigned char)alphaCompare, GX_AOP_OR, GX_NEVER, 0);
        else
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    }
    RenderContextExtensionBase::GetAlphaWritesOverride(override);
    if (override == AWO_INHERIT && alphaWrites != gCurrentState.alphaWrites) {
        GXBool update = GX_TRUE;
        if (!alphaWrites)
            update = GX_FALSE;
        GXSetAlphaUpdate(update);
    }
    gCurrentState = *this;
    return result;
}
