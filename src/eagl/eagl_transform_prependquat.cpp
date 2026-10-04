// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::PrependQuatTrans(const COORD4* q, const COORD4* t) { Transform other; other.BuildQuatTrans(q, t); PreMult(other); }
