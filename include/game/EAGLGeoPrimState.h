#ifndef GAME_EAGL_STATE_H
#define GAME_EAGL_STATE_H
#include "EAGLMemory.h"
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXPixel.h>
#include <dolphin/gx/GXCull.h>
#include <dolphin/gx/GXTev.h>
namespace EAGL {
enum PrimitiveType {
    PRIMITIVE_TRIANGLE_STRIP = 0x98,
    PRIMITIVE_POINT_LIST = 0xb8,
    PRIMITIVE_TRIANGLE_LIST = 0x90
};
enum DepthTestMethod { DTM_LEQUAL = 3 };
enum AlphaBlendMode { ABM_OFF, ABM_BLEND, ABM_ADD, ABM_ATTENUATE, ABM_MODULATE, ABM_SUBTRACT, ABM_CUSTOM };
enum AlphaTestMethod { ATM_GREATER = 4 };
enum TransparencyMethod { TM_OPAQUE, TM_ALPHA, TM_CHROMAKEY };
enum CullDirection { CD_CLOCKWISE, CD_COUNTERCLOCKWISE };
enum Shading { S_FLAT, S_GOURAUD, S_SPECULAR };
enum TextureCoordType { TCT_STQ, TCT_UV };
enum AlphaWritesOverride { AWO_INHERIT = -1 };
// Four-byte color storage is supported by argument copies and vertex-array strides.
// The original channel representation and member name are unknown.
class Colour {
  public:
    unsigned int packed;
};
// The extension occupies 60 bytes. Descriptive fields preserve observed offsets.
class GeoPrimStateExtension {
  public:
    PrimitiveType primitive;
    DepthTestMethod depthTest;
    AlphaBlendMode blend;
    int alphaCompare;
    AlphaTestMethod alphaMethod;
    TransparencyMethod transparency;
    GXBlendMode customMode;
    GXBlendFactor customSource, customDestination;
    GXLogicOp customLogic;
    int cullOverride, zWriteOverride;
    bool alphaTest, cull, alphaWrites;
    GeoPrimStateExtension();
    ~GeoPrimStateExtension();
    bool Use() const;
    static void SetRenderContextStates();
    static void SetCurrentVertex(unsigned int, unsigned int);
    static void GetCurrentVertex(unsigned int &, unsigned int &);
    static void SetAttributeFormat(int, GXAttr, GXCompCnt, GXCompType, unsigned char);
    void SetZWritesEnable(bool);
    void SetCullDirection(CullDirection);
    void SetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
    static unsigned int gVertexFormat, gVertexDataType;
    static bool gStateInitialized, gCullClockwise, gZWritesEnable, gColourWritesEnable;
    static GeoPrimStateExtension gCurrentState;
};
// Member destruction (ABI flag 2) supports composition at offset zero.
class GeoPrimState {
  public:
    GeoPrimStateExtension extension;
    GeoPrimState();
    ~GeoPrimState();
    static void *operator new(unsigned int n) { return EAGLInternal::EAGLMalloc(n, 0); }
    static void operator delete(void *p, unsigned int n) { EAGLInternal::EAGLFree(p, n); }
    bool SetShading(Shading);
    bool SetTextureEnable(bool);
    bool SetTextureCoordType(TextureCoordType);
    bool SetChromaColour(Colour);
    bool SetPrimitiveType(PrimitiveType);
    bool GetPrimitiveType(PrimitiveType &) const;
    bool SetCullEnable(bool);
    bool SetDepthTestMethod(DepthTestMethod);
    bool SetAlphaBlendMode(AlphaBlendMode);
    bool SetAlphaTestEnable(bool);
    bool SetAlphaCompareValue(int);
    bool SetAlphaTestMethod(AlphaTestMethod);
    bool SetTransparencyMethod(TransparencyMethod);
    bool GetTransparencyMethod(TransparencyMethod &) const;
};
} // namespace EAGL
#endif
