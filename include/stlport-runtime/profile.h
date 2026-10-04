#define _STLP_NO_WCHAR_T 1
#define _STLP_NO_IOSTREAMS 1
#define _STLP_USE_NEWALLOC 1
#define _STLP_NO_THREADS 1
#include <stl/_config.h>
#undef _STLP_NATIVE_C_HEADER
#define _STLP_NATIVE_C_HEADER(h) <h>
#undef _STLP_NATIVE_CPP_RUNTIME_HEADER
#define _STLP_NATIVE_CPP_RUNTIME_HEADER(h) <native/h>
#undef _STLP_USE_NEW_C_HEADERS
