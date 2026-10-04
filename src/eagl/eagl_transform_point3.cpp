// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLTransform.h"
void EAGL::Transform::TransformPoint(const COORD3& in, COORD3& out) const {
    if (&in != &out) {
        out.x  =  in.x * m[0][0] + in.y * m[1][0] + in.z * m[2][0] + m[3][0];
        out.y  =  in.x * m[0][1] + in.y * m[1][1] + in.z * m[2][1] + m[3][1];
        out.z  =  in.x * m[0][2] + in.y * m[1][2] + in.z * m[2][2] + m[3][2];
    } else {
        COORD3 temp;
        temp.x  =  in.x * m[0][0] + in.y * m[1][0] + in.z * m[2][0] + m[3][0];
        temp.y  =  in.x * m[0][1] + in.y * m[1][1] + in.z * m[2][1] + m[3][1];
        temp.z  =  in.x * m[0][2] + in.y * m[1][2] + in.z * m[2][2] + m[3][2];
        out  =  temp;
    }
}
