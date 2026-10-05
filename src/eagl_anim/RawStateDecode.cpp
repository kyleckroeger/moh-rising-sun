// AI-assisted reconstruction/adaptation; see docs/Animation.md and NOTICE.
#include "EAGLAnim/RawStateChan.h"
namespace EAGLAnim {
void FnRawStateChan::Decode(unsigned char *src, unsigned char *dest) const {
    RawStateChan *anim = reinterpret_cast<RawStateChan *>(mpAnim);
    unsigned int value = 0;
    unsigned char bit = 0;
    for (int i = 0; i < anim->mNumFields; ++i) {
        unsigned char bits, bytes, offset;
        anim->GetDecodeData(i, bits, bytes, offset);
        switch (bits) {
        case 5:
            value = *reinterpret_cast<unsigned int *>(src);
            src += 4;
            break;
        case 4:
            value = *reinterpret_cast<unsigned short *>(src);
            src += 2;
            break;
        case 3:
            value = *src++;
            break;
        case 2:
            bit += 4;
            value = (*src >> (8 - bit)) & 15;
            break;
        case 1:
            bit += 2;
            value = (*src >> (8 - bit)) & 3;
            break;
        case 0:
            ++bit;
            value = (*src >> (8 - bit)) & 1;
            break;
        }
        if (bit > 7) {
            ++src;
            bit = 0;
        }
        switch (bytes) {
        case 4:
            *reinterpret_cast<unsigned int *>(dest + offset) = value;
            break;
        case 2:
            *reinterpret_cast<unsigned short *>(dest + offset) = value;
            break;
        case 1:
            dest[offset] = value;
            break;
        }
    }
}
} // namespace EAGLAnim
