// AI-assisted reconstruction; see docs/Animation.md and NOTICE.
#include "EAGLAnim/Skeleton.h"
static void MtxMult(EAGL::Transform *output, const EAGL::Transform *left, const EAGL::Transform *right) {
    EAGL::Transform temp;
    temp.m[0][0] = right->m[0][0] * left->m[0][0] + right->m[0][1] * left->m[1][0] +
                   right->m[0][2] * left->m[2][0] + right->m[0][3] * left->m[3][0];
    temp.m[0][1] = right->m[0][0] * left->m[0][1] + right->m[0][1] * left->m[1][1] +
                   right->m[0][2] * left->m[2][1] + right->m[0][3] * left->m[3][1];
    temp.m[0][2] = right->m[0][0] * left->m[0][2] + right->m[0][1] * left->m[1][2] +
                   right->m[0][2] * left->m[2][2] + right->m[0][3] * left->m[3][2];
    temp.m[0][3] = right->m[0][0] * left->m[0][3] + right->m[0][1] * left->m[1][3] +
                   right->m[0][2] * left->m[2][3] + right->m[0][3] * left->m[3][3];
    temp.m[1][0] = right->m[1][0] * left->m[0][0] + right->m[1][1] * left->m[1][0] +
                   right->m[1][2] * left->m[2][0] + right->m[1][3] * left->m[3][0];
    temp.m[1][1] = right->m[1][0] * left->m[0][1] + right->m[1][1] * left->m[1][1] +
                   right->m[1][2] * left->m[2][1] + right->m[1][3] * left->m[3][1];
    temp.m[1][2] = right->m[1][0] * left->m[0][2] + right->m[1][1] * left->m[1][2] +
                   right->m[1][2] * left->m[2][2] + right->m[1][3] * left->m[3][2];
    temp.m[1][3] = right->m[1][0] * left->m[0][3] + right->m[1][1] * left->m[1][3] +
                   right->m[1][2] * left->m[2][3] + right->m[1][3] * left->m[3][3];
    temp.m[2][0] = right->m[2][0] * left->m[0][0] + right->m[2][1] * left->m[1][0] +
                   right->m[2][2] * left->m[2][0] + right->m[2][3] * left->m[3][0];
    temp.m[2][1] = right->m[2][0] * left->m[0][1] + right->m[2][1] * left->m[1][1] +
                   right->m[2][2] * left->m[2][1] + right->m[2][3] * left->m[3][1];
    temp.m[2][2] = right->m[2][0] * left->m[0][2] + right->m[2][1] * left->m[1][2] +
                   right->m[2][2] * left->m[2][2] + right->m[2][3] * left->m[3][2];
    temp.m[2][3] = right->m[2][0] * left->m[0][3] + right->m[2][1] * left->m[1][3] +
                   right->m[2][2] * left->m[2][3] + right->m[2][3] * left->m[3][3];
    temp.m[3][0] = right->m[3][0] * left->m[0][0] + right->m[3][1] * left->m[1][0] +
                   right->m[3][2] * left->m[2][0] + right->m[3][3] * left->m[3][0];
    temp.m[3][1] = right->m[3][0] * left->m[0][1] + right->m[3][1] * left->m[1][1] +
                   right->m[3][2] * left->m[2][1] + right->m[3][3] * left->m[3][1];
    temp.m[3][2] = right->m[3][0] * left->m[0][2] + right->m[3][1] * left->m[1][2] +
                   right->m[3][2] * left->m[2][2] + right->m[3][3] * left->m[3][2];
    temp.m[3][3] = right->m[3][0] * left->m[0][3] + right->m[3][1] * left->m[1][3] +
                   right->m[3][2] * left->m[2][3] + right->m[3][3] * left->m[3][3];
    *output = temp;
}
namespace EAGLAnim {
void (*MatrixMultiply)(EAGL::Transform *, const EAGL::Transform *, const EAGL::Transform *) = MtxMult;
}
