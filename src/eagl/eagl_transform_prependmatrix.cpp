// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::PrependMatrix(const MATRIX4* rhs) {
    Transform other  =  *(const Transform*)rhs;
    PreMult(other);
}
