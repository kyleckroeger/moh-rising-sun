#ifndef GAME_STRING_CRC_H
#define GAME_STRING_CRC_H

// Hash raw string bytes without case folding. Null and empty strings return zero.
unsigned int GetStringCRC(const char *text);

#endif
