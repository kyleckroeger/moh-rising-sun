// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::PostMult(const Transform& rhs) {
    MATRIX4 result;
    MultMatrix((const MATRIX4*)this, (const MATRIX4*)&rhs, &result);
    MEM_copy(this, &result, 64);
}
