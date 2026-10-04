// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::PreMult(const Transform& rhs) {
    MATRIX4 result;
    MultMatrix((const MATRIX4*)&rhs, (const MATRIX4*)this, &result);
    MEM_copy(this, &result, 64);
}
