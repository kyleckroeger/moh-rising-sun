// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
static inline void PostMultiply(EAGL::Transform& dst, const EAGL::Transform& rhs) { MATRIX4 result; MultMatrix((const MATRIX4*)&dst, (const MATRIX4*)&rhs, &result); MEM_copy(&dst, &result, 64); }
static inline void BuildTranslation(EAGL::Transform& other, float x, float y, float z) {
    other.m[0][0]  =  1.0f;
    other.m[0][1]  =  0.0f;
    other.m[0][2]  =  0.0f;
    other.m[0][3]  =  0.0f;
    other.m[1][0]  =  0.0f;
    other.m[1][1]  =  1.0f;
    other.m[1][2]  =  0.0f;
    other.m[1][3]  =  0.0f;
    other.m[2][0]  =  0.0f;
    other.m[2][1]  =  0.0f;
    other.m[2][2]  =  1.0f;
    other.m[2][3]  =  0.0f;
    other.m[3][0]  =  x;
    other.m[3][1]  =  y;
    other.m[3][2]  =  z;
    other.m[3][3]  =  1.0f;
}
void EAGL::Transform::AppendTranslate(float x, float y, float z) { Transform other; BuildTranslation(other, x, y, z); PostMultiply(*this, other); }
