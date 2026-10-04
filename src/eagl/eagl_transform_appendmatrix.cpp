// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
static inline void PostMultiply(EAGL::Transform& dst, const EAGL::Transform& rhs) { MATRIX4 result; MultMatrix((const MATRIX4*)&dst, (const MATRIX4*)&rhs, &result); MEM_copy(&dst, &result, 64); }
void EAGL::Transform::AppendMatrix(const MATRIX4* rhs) {
    Transform other  =  *(const Transform*)rhs;
    PostMultiply(*this, other);
}
