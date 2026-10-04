// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLPropertyParser.h"
namespace EAGLInternal {
int PropertyParser::FindTokenEnd(char *p, char *&end) {
    while (p && *p && *p != ';' && *p != ',' && *p != '=')
        ++p;
    end = p;
    if (end) {
        int c = *end;
        *end = 0;
        return c;
    }
    return 0;
}
} // namespace EAGLInternal
