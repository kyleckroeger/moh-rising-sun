// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::Transpose() {
    float temp;
    temp = m[0][1]; m[0][1] = m[1][0]; m[1][0] = temp;
    temp = m[0][2]; m[0][2] = m[2][0]; m[2][0] = temp;
    temp = m[0][3]; m[0][3] = m[3][0]; m[3][0] = temp;
    temp = m[1][2]; m[1][2] = m[2][1]; m[2][1] = temp;
    temp = m[1][3]; m[1][3] = m[3][1]; m[3][1] = temp;
    temp = m[2][3]; m[2][3] = m[3][2]; m[3][2] = temp;
}
