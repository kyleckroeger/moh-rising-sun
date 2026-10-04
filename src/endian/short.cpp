#include "Endian.h"

void ChangeEndian(short &value) {
    ChangeEndian(reinterpret_cast<unsigned short &>(value));
}
