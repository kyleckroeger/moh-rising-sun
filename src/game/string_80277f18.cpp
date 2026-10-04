// AI-assisted reconstruction; evidence and scope: docs/Observers.md.
template <class T> void CopyString(short *out, const T *in) {
    if (out) {
        if (in) {
            while (*in)
                *out++ = *in++;
        }
        *out = 0;
    }
}
template void CopyString<short>(short *, const short *);
