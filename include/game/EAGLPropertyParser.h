#ifndef GAME_EAGL_PROPERTY_PARSER_H
#define GAME_EAGL_PROPERTY_PARSER_H
#include <string.h>
#include "EAGLMemory.h"
namespace EAGLInternal {
// Descriptive four-byte argument wrapper: zero-initialized by the parser.
// Its original type/name are unknown. Native array cookies and cleanup are checked.
class PropertyArgument {
  public:
    const char *text;
    PropertyArgument() : text(0) {}
    operator const char *() const { return text; }
    PropertyArgument &operator=(const char *s) {
        text = s;
        return *this;
    }
    static void *operator new[](unsigned int n) { return EAGLMalloc(n, 0); }
    static void operator delete[](void *p, unsigned int n) { EAGLFree(p, n); }
};
class Property {
  public:
    const char *name;
    int argumentCount;
    PropertyArgument *arguments;
    Property() : name(0), argumentCount(0), arguments(0) {}
    ~Property() { delete[] arguments; }
    static void *operator new[](unsigned int n) { return EAGLMalloc(n, 0); }
    static void operator delete[](void *p, unsigned int n) { EAGLFree(p, n); }
    void AllocateArguments(int count) { arguments = new PropertyArgument[count]; }
    bool Matches(const char *n, int count) const {
        if (count == argumentCount)
            return strcmp(n, name) == 0;
        return false;
    }
    bool Matches(const char *n) const { return strcmp(n, name) == 0; }
    bool StartsWith(const char *n) const { return strncmp(n, name, strlen(n)) == 0; }
};
class PropertyParser {
  public:
    int count;
    Property *properties;
    int textLength;
    char *text;
    const Property &GetProperty(int i) const { return properties[i]; }
    int GetCount() const { return count; }
    int MAXINT(int, int);
    char *SkipDelims(char *);
    int FindTokenEnd(char *, char *&);
    PropertyParser(const char *source) : count(0), properties(0) {
        textLength = strlen(source);
        text = (char *)EAGLMalloc(textLength + 1, 0);
        strcpy(text, source);
        int numProperties = 0, maxArguments = 0, numArguments = 0;
        const char *scan = text;
        int i;
        for (i = 0; i < textLength; ++i) {
            if (scan[i] == ',')
                ++numArguments;
            maxArguments = MAXINT(numArguments, maxArguments);
            if (scan[i] == ';') {
                ++numProperties;
                numArguments = 0;
            }
        }
        ++numProperties;
        ++maxArguments;
        properties = new Property[numProperties];
        for (i = 0; i < numProperties; ++i)
            properties[i].AllocateArguments(maxArguments);
        char *start = SkipDelims(text);
        char *end;
        int property = 0, arg = 0;
        bool readingArgs = false;
        int delimiter;
        do {
            delimiter = FindTokenEnd(start, end);
            switch (delimiter) {
            case '=':
                if (!readingArgs) {
                    properties[property].name = start;
                    arg = 0;
                    readingArgs = true;
                }
                break;
            case ',':
                if (readingArgs)
                    properties[property].arguments[arg++] = start;
                break;
            case ';':
                if (readingArgs) {
                    properties[property].arguments[arg++] = start;
                    properties[property++].argumentCount = arg;
                }
                readingArgs = false;
                break;
            case 0:
                if (readingArgs) {
                    properties[property].arguments[arg++] = start;
                    properties[property++].argumentCount = arg;
                }
                break;
            }
            start = end + 1;
        } while (delimiter);
        count = property;
    }
    ~PropertyParser() {
        delete[] properties;
        EAGLFree(text, textLength + 1);
    }
};
} // namespace EAGLInternal
#endif
