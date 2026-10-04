#ifndef GAME_ENDIAN_H
#define GAME_ENDIAN_H

// Scoped original overload interface. Unsigned and template implementations
// remain external; see docs/BPD.md for the accepted scalar wrappers.
void ChangeEndian(unsigned int &);
void ChangeEndian(float &);
void ChangeEndian(int &);
void ChangeEndian(unsigned short &);
void ChangeEndian(short &);
template <class T> void ChangeEndian(T &);
#endif
