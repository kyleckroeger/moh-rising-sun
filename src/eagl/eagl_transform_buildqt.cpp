// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::BuildQT(float x, float y, float z, float w, float tx, float ty, float tz) {
    float x2 = x+x, y2 = y+y, z2 = z+z;
    float wx = w*x2, wy = w*y2, wz = w*z2;
    float xx = x*x2, xy = x*y2, xz = x*z2;
    float yy = y*y2, yz = y*z2, zz = z*z2;
    m[0][0] = 1.0f-(yy+zz);
    m[1][0] = xy-wz;
    m[2][0] = xz+wy;
    m[0][1] = xy+wz;
    m[1][1] = 1.0f-(xx+zz);
    m[2][1] = yz-wx;
    m[0][2] = xz-wy;
    m[1][2] = yz+wx;
    m[2][2] = 1.0f-(xx+yy);
    m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f;
    m[3][0] = tx; m[3][1] = ty; m[3][2] = tz; m[3][3] = 1.0f;
}
