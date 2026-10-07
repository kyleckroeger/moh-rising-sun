// AI-assisted reconstruction; see docs/LightVolumes.md.
// Private light.cpp helper used by CLight::SetLightBlock (not yet reconstructed).
static float Clamp0to1(const float &value) {
    if (value > 1.0f)
        return 1.0f;
    if (value >= 0.0f)
        return value;
    return 0.0f;
}
