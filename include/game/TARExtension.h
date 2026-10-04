#ifndef GAME_EAGL_TAR_H
#define GAME_EAGL_TAR_H
#include <dolphin/gx/GXEnum.h>
// TexGenState is a descriptive storage view; TARExtension has no instance layout here.
namespace EAGL {
struct TexGenState {
    GXTexGenType type[8];
    GXTexGenSrc source[8];
    unsigned int matrix[8];
    unsigned char normalize[8];
    unsigned int postMatrix[8];
};
class TARExtension {
public:
    void Use(GXTexMapID, unsigned int);
    static int gNumTexGens;
    static TexGenState gTexGens;
    static void SetNumTexGens(int);
    static void SetTexCoordGen(GXTexCoordID, GXTexGenType, GXTexGenSrc, unsigned int, unsigned char, unsigned int);
    static void ResetTexCoordGens();
};
}
#endif
