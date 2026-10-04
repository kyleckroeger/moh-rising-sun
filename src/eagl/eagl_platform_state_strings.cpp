// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include <string.h>
namespace EAGL { int PrintMessage(int, const char*, ...); }
namespace EAGLInternal {
int StringToPlatformStateEnum(const char* text) {
    if (strncmp(text, "EAGL::GCBM_", 11) == 0) {
        if (strcmp(text, "EAGL::GCBM_NONE") == 0) return 0;
        if (strcmp(text, "EAGL::GCBM_BLEND") == 0) return 1;
        if (strcmp(text, "EAGL::GCBM_LOGIC") == 0) return 2;
        if (strcmp(text, "EAGL::GCBM_SUBTRACT") == 0) return 3;
        if (strcmp(text, "EAGL::GCBM_MAX") == 0) return 4;
    }
    else if (strncmp(text, "EAGL::GCBL_", 11) == 0) {
        if (strcmp(text, "EAGL::GCBL_ZERO") == 0) return 0;
        if (strcmp(text, "EAGL::GCBL_ONE") == 0) return 1;
        if (strcmp(text, "EAGL::GCBL_SRCCLR") == 0) return 2;
        if (strcmp(text, "EAGL::GCBL_INVSRCCLR") == 0) return 3;
        if (strcmp(text, "EAGL::GCBL_SRCALPHA") == 0) return 4;
        if (strcmp(text, "EAGL::GCBL_INVSRCALPHA") == 0) return 5;
        if (strcmp(text, "EAGL::GCBL_DSTALPHA") == 0) return 6;
        if (strcmp(text, "EAGL::GCBL_INVDSTALPHA") == 0) return 7;
        if (strcmp(text, "EAGL::GCBL_DSTCLR") == 0) return 2;
        if (strcmp(text, "EAGL::GCBL_INVDSTCLR") == 0) return 3;
    }
    else if (strncmp(text, "EAGL::GCLO_", 11) == 0) {
        if (strcmp(text, "EAGL::GCLO_CLEAR") == 0) return 0;
        if (strcmp(text, "EAGL::GCLO_AND") == 0) return 1;
        if (strcmp(text, "EAGL::GCLO_REVAND") == 0) return 2;
        if (strcmp(text, "EAGL::GCLO_COPY") == 0) return 3;
        if (strcmp(text, "EAGL::GCLO_INVAND") == 0) return 4;
        if (strcmp(text, "EAGL::GCLO_NOOP") == 0) return 5;
        if (strcmp(text, "EAGL::GCLO_XOR") == 0) return 6;
        if (strcmp(text, "EAGL::GCLO_OR") == 0) return 7;
        if (strcmp(text, "EAGL::GCLO_NOR") == 0) return 8;
        if (strcmp(text, "EAGL::GCLO_EQUIV") == 0) return 9;
        if (strcmp(text, "EAGL::GCLO_INV") == 0) return 10;
        if (strcmp(text, "EAGL::GCLO_REVOR") == 0) return 11;
        if (strcmp(text, "EAGL::GCLO_INVCOPY") == 0) return 12;
        if (strcmp(text, "EAGL::GCLO_INVOR") == 0) return 13;
        if (strcmp(text, "EAGL::GCLO_NAND") == 0) return 14;
        if (strcmp(text, "EAGL::GCLO_SET") == 0) return 15;
    }
    else if (strncmp(text, "EAGL::GCA_", 10) == 0) {
        if (strcmp(text, "EAGL::GCA_VA_PNMTXIDX") == 0) return 0;
        if (strcmp(text, "EAGL::GCA_VA_TEX0MTXIDX") == 0) return 1;
        if (strcmp(text, "EAGL::GCA_VA_TEX1MTXIDX") == 0) return 2;
        if (strcmp(text, "EAGL::GCA_VA_TEX2MTXIDX") == 0) return 3;
        if (strcmp(text, "EAGL::GCA_VA_TEX3MTXIDX") == 0) return 4;
        if (strcmp(text, "EAGL::GCA_VA_TEX4MTXIDX") == 0) return 5;
        if (strcmp(text, "EAGL::GCA_VA_TEX5MTXIDX") == 0) return 6;
        if (strcmp(text, "EAGL::GCA_VA_TEX6MTXIDX") == 0) return 7;
        if (strcmp(text, "EAGL::GCA_VA_TEX7MTXIDX") == 0) return 8;
        if (strcmp(text, "EAGL::GCA_VA_POS") == 0) return 9;
        if (strcmp(text, "EAGL::GCA_VA_NRM") == 0) return 10;
        if (strcmp(text, "EAGL::GCA_VA_CLR0") == 0) return 11;
        if (strcmp(text, "EAGL::GCA_VA_CLR1") == 0) return 12;
        if (strcmp(text, "EAGL::GCA_VA_TEX0") == 0) return 13;
        if (strcmp(text, "EAGL::GCA_VA_TEX1") == 0) return 14;
        if (strcmp(text, "EAGL::GCA_VA_TEX2") == 0) return 15;
        if (strcmp(text, "EAGL::GCA_VA_TEX3") == 0) return 16;
        if (strcmp(text, "EAGL::GCA_VA_TEX4") == 0) return 17;
        if (strcmp(text, "EAGL::GCA_VA_TEX5") == 0) return 18;
        if (strcmp(text, "EAGL::GCA_VA_TEX6") == 0) return 19;
        if (strcmp(text, "EAGL::GCA_VA_TEX7") == 0) return 20;
        if (strcmp(text, "EAGL::GCA_VA_POS_MTX_ARRAY") == 0) return 21;
        if (strcmp(text, "EAGL::GCA_VA_NRM_MTX_ARRAY") == 0) return 22;
        if (strcmp(text, "EAGL::GCA_VA_TEX_MTX_ARRAY") == 0) return 23;
        if (strcmp(text, "EAGL::GCA_VA_LIGHT_ARRAY") == 0) return 24;
        if (strcmp(text, "EAGL::GCA_VA_NBT") == 0) return 25;
        if (strcmp(text, "EAGL::GCA_VA_MAX_ATTR") == 0) return 26;
        if (strcmp(text, "EAGL::GCA_VA_NULL") == 0) return 255;
    }
    else if (strncmp(text, "EAGL::GCCC_", 11) == 0) {
        if (strcmp(text, "EAGL::GCCC_POS_XY") == 0) return 0;
        if (strcmp(text, "EAGL::GCCC_POS_XYZ") == 0) return 1;
        if (strcmp(text, "EAGL::GCCC_NRM_XYZ") == 0) return 0;
        if (strcmp(text, "EAGL::GCCC_NRM_NBT") == 0) return 1;
        if (strcmp(text, "EAGL::GCCC_NRM_NBT3") == 0) return 2;
        if (strcmp(text, "EAGL::GCCC_CLR_RGB") == 0) return 0;
        if (strcmp(text, "EAGL::GCCC_CLR_RGBA") == 0) return 1;
        if (strcmp(text, "EAGL::GCCC_TEX_S") == 0) return 0;
        if (strcmp(text, "EAGL::GCCC_TEX_ST") == 0) return 1;
    }
    else if (strncmp(text, "EAGL::GCCT_", 11) == 0) {
        if (strcmp(text, "EAGL::GCCT_U8") == 0) return 0;
        if (strcmp(text, "EAGL::GCCT_S8") == 0) return 1;
        if (strcmp(text, "EAGL::GCCT_U16") == 0) return 2;
        if (strcmp(text, "EAGL::GCCT_S16") == 0) return 3;
        if (strcmp(text, "EAGL::GCCT_F32") == 0) return 4;
        if (strcmp(text, "EAGL::GCCT_RGB565") == 0) return 0;
        if (strcmp(text, "EAGL::GCCT_RGB8") == 0) return 1;
        if (strcmp(text, "EAGL::GCCT_RGBX8") == 0) return 2;
        if (strcmp(text, "EAGL::GCCT_RGBA4") == 0) return 3;
        if (strcmp(text, "EAGL::GCCT_RGBA6") == 0) return 4;
        if (strcmp(text, "EAGL::GCCT_RGBA8") == 0) return 5;
    }
    EAGL::PrintMessage(0, "INTERNAL ERROR: Invalid GeoPrimState value %s\n", text);
    return 0;
}
}
