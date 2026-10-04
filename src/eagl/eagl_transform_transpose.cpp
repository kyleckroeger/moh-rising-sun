// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::Transpose(const Transform& src, Transform& dst) {
    Transform tmp;
    Transform* output = &dst;
    if (&src==&dst) output = &tmp;
    output->m[0][0] = src.m[0][0];
    output->m[0][1] = src.m[1][0];
    output->m[0][2] = src.m[2][0];
    output->m[0][3] = src.m[3][0];
    output->m[1][0] = src.m[0][1];
    output->m[1][1] = src.m[1][1];
    output->m[1][2] = src.m[2][1];
    output->m[1][3] = src.m[3][1];
    output->m[2][0] = src.m[0][2];
    output->m[2][1] = src.m[1][2];
    output->m[2][2] = src.m[2][2];
    output->m[2][3] = src.m[3][2];
    output->m[3][0] = src.m[0][3];
    output->m[3][1] = src.m[1][3];
    output->m[3][2] = src.m[2][3];
    output->m[3][3] = src.m[3][3];
    if (&src==&dst) dst = tmp;
}
