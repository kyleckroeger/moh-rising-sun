// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
#include <dolphin/gx/GXGeometry.h>
namespace EAGL {
void TARExtension::SetNumTexGens(int n) {
    gNumTexGens  =  n;
    GXSetNumTexGens(n);
}
void TARExtension::SetTexCoordGen(GXTexCoordID id, GXTexGenType type, GXTexGenSrc source, unsigned int matrix, unsigned char normalize, unsigned int postMatrix) {
    gTexGens.type[id]  =  type;
    gTexGens.source[id]  =  source;
    gTexGens.matrix[id]  =  matrix;
    gTexGens.normalize[id]  =  normalize;
    gTexGens.postMatrix[id]  =  postMatrix;
    GXSetTexCoordGen2(id, gTexGens.type[id], gTexGens.source[id], gTexGens.matrix[id], gTexGens.normalize[id], gTexGens.postMatrix[id]);
}
void TARExtension::ResetTexCoordGens() {
    for (int i  =  0; i < 8; ++i) {
        gTexGens.type[i]  =  GX_TG_MTX2x4;
        gTexGens.source[i]  =  (GXTexGenSrc)(i + GX_TG_TEX0);
        gTexGens.matrix[i]  =  GX_IDENTITY;
        gTexGens.normalize[i]  =  0;
        gTexGens.postMatrix[i]  =  GX_PTIDENTITY;
    }
}
}
