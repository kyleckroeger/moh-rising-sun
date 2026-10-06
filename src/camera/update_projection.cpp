// Reconstructed from GR8E69 with Codex assistance; see docs/Camera.md.
#include "Camera.h"

void CCamera::UpdateCameraToClip() {
    if (perspective)
        cameraToClip.Perspective(projectionX, projectionY, nearClip, farClip * 100.0f);
    else
        cameraToClip.Orthographic(projectionX, projectionY, nearClip, farClip);
    cameraToClipValid = 1;
}
