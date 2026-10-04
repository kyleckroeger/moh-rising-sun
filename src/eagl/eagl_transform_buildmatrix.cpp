// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::BuildMatrix(const MATRIX4* rhs) { *(MATRIX4*)this  =  *rhs; }
