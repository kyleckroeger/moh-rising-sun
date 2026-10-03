# GCC runtime restoration

Eight runtime helpers restore **5,532 code bytes** from [GCC 2.95.2](https://github.com/gcc-mirror/gcc/tree/releases/gcc-2.95.2/gcc). They implement signed and unsigned 64-bit division/remainder, three 64-bit shifts and signed-64-to-float conversion. The entire function bodies match the pinned executable. They are restored library code, with AI assistance used for target adaptation and verification.

## Source and target configuration

The [provenance manifest](../config/GR8E69/libgcc-source.json) records each source URL and SHA-256. Upstream notices, the GNU GPL v2 and libgcc2.c's runtime-linking exception are preserved. The target configuration supplies the observed big-endian 32-bit PowerPC ABI and selects upstream portable C arithmetic with `NO_ASM`. No assembly arithmetic implementations are compiled or credited. Only the eight selected `L_` variants are verified.

The original ELF identifies four separate private `__clz_tab` symbols, each 256 bytes, at `0x804529f8`, `0x80452af8`, `0x80452bf8` and `0x80452cf8`. Each division/remainder unit owns one table. Their definitions and declarations receive a `.sdata2` section attribute, matching that explicit symbol/section evidence. This is a target storage annotation, not a recovered historical spelling. Table contents remain unchanged upstream data. The float-conversion helper also reproduces its 24-byte read-only constant pool.

## Build and verification

All eight use ProDG 3.8.1 with `-O2 -G8`, selecting one upstream `L_` variant per object. The exact historical compiler release remains unproven. Native SN linking resolves the original small-data references, followed by the same complete object, linked-section and full-image checks used throughout the project. Each retained function is counted once; the 1,048 generated data bytes earn no executable-code credit. The source stays shared rather than duplicating the implementation for each build variant.

Runtime has not been tested. Unselected libgcc code and target configurations remain unverified.
