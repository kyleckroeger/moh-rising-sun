// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::OrthoInverse() {
    float t;
    t = m[0][1]; m[0][1] = m[1][0]; m[1][0] = t;
    t = m[0][2]; m[0][2] = m[2][0]; m[2][0] = t;
    t = m[1][2]; m[1][2] = m[2][1]; m[2][1] = t;
    float x  =  -m[3][0] * m[0][0] - m[3][1] * m[1][0] - m[3][2] * m[2][0];
    float y  =  -m[3][0] * m[0][1] - m[3][1] * m[1][1] - m[3][2] * m[2][1];
    float z  =  -m[3][0] * m[0][2] - m[3][1] * m[1][2] - m[3][2] * m[2][2];
    m[3][0] = x; m[3][1] = y; m[3][2] = z;
}
