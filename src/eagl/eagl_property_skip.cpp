// AI-assisted reconstruction from GR8E69; see docs/Rendering.md.
#include "EAGLGeoPrimState.h"
#include "EAGLPropertyParser.h"
namespace EAGLInternal {
char *PropertyParser::SkipDelims(char *p) {
    while (p && *p && (*p == ';' || *p == ',' || *p == '='))
        ++p;
    return p;
}
} // namespace EAGLInternal
