// Reconstructed from GR8E69 with Codex assistance; see docs/Camera.md.
#include "Camera.h"

#include <math.h>

void CCamera::Orthonormalize() {
    localToWorld.Orthonormalize();
    worldToClipValid = worldToCameraValid = 0;
}

void CCamera::SetPerspective(float horizontalHalfAngle, float verticalHalfAngle) {
    projectionX = tanf(horizontalHalfAngle);
    projectionY = tanf(verticalHalfAngle);
    cameraToClipValid = 0;
    worldToClipValid = 0;
    perspective = 1;
    field_160 = 1;
}

void CCamera::SetOrthographic(float x, float y) {
    projectionX = x;
    projectionY = y;
    cameraToClipValid = 0;
    worldToClipValid = 0;
    perspective = 0;
    field_160 = 1;
}

float CCamera::GetHFOV() const { return 2.0f * atanf(projectionX); }

float CCamera::GetVFOV() const { return 2.0f * atanf(projectionY); }
