#include "Endian.h"

void ChangeEndian(float &value) {
    ChangeEndian(reinterpret_cast<unsigned int &>(value));
}

void ChangeEndian(int &value) {
    ChangeEndian(reinterpret_cast<unsigned int &>(value));
}
