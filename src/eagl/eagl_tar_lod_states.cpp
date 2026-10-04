#include "EAGLMaterial.h"
// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "TARExtension.h"
using namespace EAGL;
void TARExtension::SetTextureLODStates() {
    Device::Get()->GetCurrentRenderContext();
    int maxLOD = numMipmaps - 1;
    GXTexFilter minFilter = GX_LINEAR;
    if (maxLOD != 0)
        minFilter = GX_LIN_MIP_NEAR;
    MipMapModeOverride mipmapOverride;
    RenderContextExtensionBase::GetMipMapModeOverride(mipmapOverride);
    int mode = mipmapOverride == -1 ? mipmapMode : mipmapOverride;
    switch (mode) {
    case 0:
        maxLOD = 0;
        break;
    case 2:
        if (!usesPalette) {
            minFilter = GX_LINEAR;
            if (maxLOD != 0)
                minFilter = GX_LIN_MIP_LIN;
        }
        break;
    }
    FilterModeOverride filterOverride;
    RenderContextExtensionBase::GetFilterModeOverride(filterOverride);
    float biasOverride;
    RenderContextExtensionBase::GetMipmapLODBiasOverride(biasOverride);
    FilterMode filter = filterMode;
    float bias = lodBias;
    switch (filterOverride) {
    case 1:
        filter = FM_POINT;
        break;
    case 2:
        filter = FM_BILINEAR;
        break;
    case 3:
        filter = FM_ANISOTROPIC;
        break;
    }
    if (biasOverride != 0.0f)
        bias = biasOverride;
    GXAnisotropy anisotropy = GX_ANISO_4;
    AnisotropyOverride anisotropyOverride;
    RenderContextExtensionBase::GetMaxAnisotropyOverride(anisotropyOverride);
    switch (anisotropyOverride) {
    case 0:
        anisotropy = GX_ANISO_1;
        break;
    case 1:
        anisotropy = GX_ANISO_2;
        break;
    case 2:
        anisotropy = GX_ANISO_4;
        break;
    }
    switch (filter) {
    case FM_POINT:
        GXInitTexObjLOD(&texture, GX_NEAR, GX_NEAR, 0.0f, 0.0f, bias, lodBiasClamp, GX_FALSE, GX_ANISO_1);
        break;
    case FM_BILINEAR:
        GXInitTexObjLOD(&texture, minFilter, GX_LINEAR, 0.0f, maxLOD, bias, lodBiasClamp, GX_FALSE,
                        GX_ANISO_1);
        break;
    case FM_ANISOTROPIC:
        GXInitTexObjLOD(&texture, minFilter, GX_LINEAR, 0.0f, maxLOD, bias, lodBiasClamp, GX_FALSE,
                        anisotropy);
        break;
    }
}
