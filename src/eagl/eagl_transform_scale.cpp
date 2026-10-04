// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::BuildScale(float x, float y, float z, float w) {
    m[0][0]  =  x;
    m[0][1]  =  0.0f;
    m[0][2]  =  0.0f;
    m[0][3]  =  0.0f;
    m[1][0]  =  0.0f;
    m[1][1]  =  y;
    m[1][2]  =  0.0f;
    m[1][3]  =  0.0f;
    m[2][0]  =  0.0f;
    m[2][1]  =  0.0f;
    m[2][2]  =  z;
    m[2][3]  =  0.0f;
    m[3][0]  =  0.0f;
    m[3][1]  =  0.0f;
    m[3][2]  =  0.0f;
    m[3][3]  =  w;
}
