#include "StringCRC.h"

// Original private table remains external binary context and earns no source credit.
extern const unsigned int crcTable[256];
unsigned int GetStringCRC(const char *text) {
    unsigned int crc = 0;
    if (text) {
        unsigned int ch = static_cast<unsigned char>(*text);
        if (ch) {
            crc = ~0u;
            do {
                crc = crcTable[(ch ^ crc) & 255] ^ (crc >> 8);
                ch = static_cast<unsigned char>(*++text);
            } while (ch);
            crc = ~crc;
        }
    }
    return crc;
}
