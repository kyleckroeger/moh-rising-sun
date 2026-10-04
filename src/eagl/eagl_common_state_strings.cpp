// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include <string.h>
namespace EAGL { int PrintMessage(int, const char*, ...); }
namespace EAGLInternal {
int StringToCommonStateEnum(const char* text) {
    if (strncmp(text, "EAGL::PT_", 9) == 0) {
        if (strcmp(text, "EAGL::PT_POINTLIST") == 0) return 184;
        if (strcmp(text, "EAGL::PT_LINELIST") == 0) return 168;
        if (strcmp(text, "EAGL::PT_LINESTRIP") == 0) return 176;
        if (strcmp(text, "EAGL::PT_LINELOOP") == 0) return -1;
        if (strcmp(text, "EAGL::PT_TRIANGLELIST") == 0) return 144;
        if (strcmp(text, "EAGL::PT_TRIANGLESTRIP") == 0) return 152;
        if (strcmp(text, "EAGL::PT_TRIANGLEFAN") == 0) return 160;
        if (strcmp(text, "EAGL::PT_QUADLIST") == 0) return 128;
        if (strcmp(text, "EAGL::PT_QUADSTRIP") == 0) return -2;
        if (strcmp(text, "EAGL::PT_POLYGON") == 0) return -3;
        if (strcmp(text, "EAGL::PT_SPRITE") == 0) return -4;
    }
    else if (strncmp(text, "EAGL::S_", 8) == 0) {
        if (strcmp(text, "EAGL::S_FLAT") == 0) return 0;
        if (strcmp(text, "EAGL::S_GOURAUD") == 0) return 1;
        if (strcmp(text, "EAGL::S_SPECULAR") == 0) return 2;
    }
    else if (strncmp(text, "EAGL::ABM_", 10) == 0) {
        if (strcmp(text, "EAGL::ABM_OFF") == 0) return 0;
        if (strcmp(text, "EAGL::ABM_BLEND") == 0) return 1;
        if (strcmp(text, "EAGL::ABM_ADD") == 0) return 2;
        if (strcmp(text, "EAGL::ABM_ATTENUATE") == 0) return 3;
        if (strcmp(text, "EAGL::ABM_MODULATE") == 0) return 4;
        if (strcmp(text, "EAGL::ABM_SUBTRACT") == 0) return 5;
        if (strcmp(text, "EAGL::ABM_CUSTOM") == 0) return 6;
    }
    else if (strncmp(text, "EAGL::ATM_", 10) == 0) {
        if (strcmp(text, "EAGL::ATM_NEVER") == 0) return 0;
        if (strcmp(text, "EAGL::ATM_ALWAYS") == 0) return 7;
        if (strcmp(text, "EAGL::ATM_LESS") == 0) return 1;
        if (strcmp(text, "EAGL::ATM_LEQUAL") == 0) return 3;
        if (strcmp(text, "EAGL::ATM_EQUAL") == 0) return 2;
        if (strcmp(text, "EAGL::ATM_GEQUAL") == 0) return 6;
        if (strcmp(text, "EAGL::ATM_GREATER") == 0) return 4;
        if (strcmp(text, "EAGL::ATM_NOTEQUAL") == 0) return 5;
    }
    else if (strncmp(text, "EAGL::TM_", 9) == 0) {
        if (strcmp(text, "EAGL::TM_OPAQUE") == 0) return 0;
        if (strcmp(text, "EAGL::TM_ALPHA") == 0) return 1;
        if (strcmp(text, "EAGL::TM_CHROMAKEY") == 0) return 2;
    }
    else if (strncmp(text, "EAGL::DTM_", 10) == 0) {
        if (strcmp(text, "EAGL::DTM_NEVER") == 0) return 0;
        if (strcmp(text, "EAGL::DTM_ALWAYS") == 0) return 7;
        if (strcmp(text, "EAGL::DTM_NOTEQUAL") == 0) return 5;
        if (strcmp(text, "EAGL::DTM_LESS") == 0) return 1;
        if (strcmp(text, "EAGL::DTM_LEQUAL") == 0) return 3;
        if (strcmp(text, "EAGL::DTM_EQUAL") == 0) return 2;
        if (strcmp(text, "EAGL::DTM_GEQUAL") == 0) return 6;
        if (strcmp(text, "EAGL::DTM_GREATER") == 0) return 4;
    }
    else if (strncmp(text, "EAGL::TCT_", 10) == 0) {
        if (strcmp(text, "EAGL::TCT_STQ") == 0) return 0;
        if (strcmp(text, "EAGL::TCT_UV") == 0) return 1;
    }
    else if (strncmp(text, "EAGL::CD_", 9) == 0) {
        if (strcmp(text, "EAGL::CD_CLOCKWISE") == 0) return 0;
        if (strcmp(text, "EAGL::CD_COUNTERCLOCKWISE") == 0) return 1;
    }
    else if (strncmp(text, "EAGL::DBT_", 10) == 0) {
        if (strcmp(text, "EAGL::DBT_NONE") == 0) return 0;
        if (strcmp(text, "EAGL::DBT_Z") == 0) return 1;
        if (strcmp(text, "EAGL::DBT_W") == 0) return 2;
    }
    EAGL::PrintMessage(0, "INTERNAL ERROR: Invalid GeoPrimState function parameter '%s'\n", text);
    return 0;
}
}
