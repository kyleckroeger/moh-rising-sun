// AI-assisted reconstruction from GR8E69; see docs/Camera.md.
#include "Camera.h"
// Descriptive inline helper; preserves the original row-by-row store order.
static inline void Multiply(CMatrix &out, const CMatrix &a, const CMatrix &b) {
    out.row[0].x = a.row[0].x * b.row[0].x + a.row[0].y * b.row[1].x + a.row[0].z * b.row[2].x + a.row[0].w * b.row[3].x;
    out.row[0].y = a.row[0].x * b.row[0].y + a.row[0].y * b.row[1].y + a.row[0].z * b.row[2].y + a.row[0].w * b.row[3].y;
    out.row[0].z = a.row[0].x * b.row[0].z + a.row[0].y * b.row[1].z + a.row[0].z * b.row[2].z + a.row[0].w * b.row[3].z;
    out.row[0].w = a.row[0].x * b.row[0].w + a.row[0].y * b.row[1].w + a.row[0].z * b.row[2].w + a.row[0].w * b.row[3].w;
    out.row[1].x = a.row[1].x * b.row[0].x + a.row[1].y * b.row[1].x + a.row[1].z * b.row[2].x + a.row[1].w * b.row[3].x;
    out.row[1].y = a.row[1].x * b.row[0].y + a.row[1].y * b.row[1].y + a.row[1].z * b.row[2].y + a.row[1].w * b.row[3].y;
    out.row[1].z = a.row[1].x * b.row[0].z + a.row[1].y * b.row[1].z + a.row[1].z * b.row[2].z + a.row[1].w * b.row[3].z;
    out.row[1].w = a.row[1].x * b.row[0].w + a.row[1].y * b.row[1].w + a.row[1].z * b.row[2].w + a.row[1].w * b.row[3].w;
    out.row[2].x = a.row[2].x * b.row[0].x + a.row[2].y * b.row[1].x + a.row[2].z * b.row[2].x + a.row[2].w * b.row[3].x;
    out.row[2].y = a.row[2].x * b.row[0].y + a.row[2].y * b.row[1].y + a.row[2].z * b.row[2].y + a.row[2].w * b.row[3].y;
    out.row[2].z = a.row[2].x * b.row[0].z + a.row[2].y * b.row[1].z + a.row[2].z * b.row[2].z + a.row[2].w * b.row[3].z;
    out.row[2].w = a.row[2].x * b.row[0].w + a.row[2].y * b.row[1].w + a.row[2].z * b.row[2].w + a.row[2].w * b.row[3].w;
    out.row[3].x = a.row[3].x * b.row[0].x + a.row[3].y * b.row[1].x + a.row[3].z * b.row[2].x + a.row[3].w * b.row[3].x;
    out.row[3].y = a.row[3].x * b.row[0].y + a.row[3].y * b.row[1].y + a.row[3].z * b.row[2].y + a.row[3].w * b.row[3].y;
    out.row[3].z = a.row[3].x * b.row[0].z + a.row[3].y * b.row[1].z + a.row[3].z * b.row[2].z + a.row[3].w * b.row[3].z;
    out.row[3].w = a.row[3].x * b.row[0].w + a.row[3].y * b.row[1].w + a.row[3].z * b.row[2].w + a.row[3].w * b.row[3].w;
}
void CCamera::UpdateWorldToClip() {
    if (!cameraToClipValid) UpdateCameraToClip();
    Multiply(worldToClip, worldToCamera, cameraToClip);
    frustum.Transform(localToWorld);
    worldToClipValid = 1;
}
