# Newlib restoration evidence

The 99 accepted units/fragments restore **53,892 code bytes in 120 functions**: 55 libc units contribute 16,468 bytes in 66 functions, and 41 libm units contribute 21,028 bytes in 41 functions; three SN library variants/fragments add 16,396 bytes in 13 functions. Every retained function and generated data section matches the original executable. These bytes are `restored_library`, separately from reconstructed game code. Source adaptation and verification used AI assistance; upstream implementation credit remains with the original authors.

## Provenance

The working reference is [Newlib 1.8.2](https://www.sourceware.org/ftp/newlib/newlib-1.8.2.tar.gz), SHA-256 `e52667ed78595dd1919f4a303f8b658b1d57b926e085e8021851b279143cf3ef`. Files under `src/newlib` and `include/newlib` retain their upstream notices. [COPYING.NEWLIB](../src/newlib/COPYING.NEWLIB) is copied unchanged. This project includes software developed by the University of California, Berkeley, and at Cygnus Solutions. [The provenance manifest](../config/GR8E69/newlib-source.json) lists each reference path and digest and marks modifications.

The reference version is supported by whole-function comparisons, including the older floating-input parser in `vfscanf.c`. Newlib 1.9.0's parser emits different code and references an absent dependency. This does not establish the exact historical Newlib release or the origin of every target customization.

The compiler support directory contains GCC 2.95.2's `stdarg.h` and an adapted EGCS 1.1.2 `va-ppc.h`. The latter retains the older PowerPC `va_arg` implementation, while its `va_start` uses the GCC 2.95.2 `__builtin_saveregs` copy implementation required by the working compiler. This combination reproduces the target's register-save handling and argument traversal in the accepted printf/scanf wrappers and `__svfscanf`. It is a verified compatibility header, not a recovered historical header version. Source URLs, hashes and attribution are recorded separately; minimal integer/float/type headers supply the target's 32-bit ABI declarations. Unused header facilities remain unverified.

## Working build profile

The 55 libc units use ProDG 3.8.1 with `-O2 -G8 -fsigned-char -DMB_CAPABLE`. The five reentrant syscall wrappers additionally define upstream `MISSING_SYSCALL_NAMES`, selecting the target's public syscall names. The exact historical compiler release remains unproven. Native linking uses `--strip-unused --fix-sda`; the accepted library's small-data references resolve through the same typed, zero-byte external anchors used for Dolphin units. No source instructions are patched. Anchors must retain their original symbol values, and every final relocation must be resolved.

The math profile is `-O2 -G1024 -fno-builtin -msafe-sda`, with explicit aliases from retained `__ieee754_*` entry names to the target's public names. `-msafe-sda` places generated floating literals in small data; named arrays and every generated constant are compared. Thirty-nine unmodified math implementations use the compiler's C++ front end (`-xc++`) with a guarded `extern "C"` wrapper in fdlibm.h. This preserves C linkage and avoids extra unused scalar objects. It is a working language profile, not proof that the original library used C++. The two fmod implementations instead compile as C with the storage adaptation below.

Each `newlib_*.json` manifest enumerates complete function bodies, data ownership, retained exports, removed upstream functions and dependencies. Ownership is based on original symbols and actual compiled function ranges; a nearby `gcc2_compiled.` marker alone is not treated as proof of a complete source boundary. Unused reentrant variants may be stripped by the native linker and earn no progress credit.

## Observed adaptations

- The target `FILE` has a ten-byte `_ubuf`, producing the observed 96-byte structure size and `_data` offset 92. Original `ungetc` accesses and `_fwalk`'s 96-byte stride provide independent evidence. No unknown padding was added to force the layout.
- Stream initialization calls `_sn_sinit` using the observed unspecified-argument declaration and passes the reentrancy pointer. The upstream `__sinit` symbol is absent. The declaration preserves the target call sequence; the initializer itself remains original context.
- The target uses a fixed ungetc buffer. `FREEUB` performs no work, and `ungetc` reports overflow through `write` and returns EOF instead of allocating a larger buffer. Its full error string, including the original write length, is compared. The absent `__submore` allocator helper is omitted.
- `__smakebuf` selects the stream's built-in one-byte unbuffered storage after the observed file-status checks. The later allocation/isatty path is absent from the original implementation.
- `fread` uses the original global `memcpyalpha` byte-copy helper. Its name, 44-byte function body, placement and two call sites are present in the original ELF; the helper is reconstructed in C and both functions match together.

The original five libc adaptations are `sys/reent.h`, `stdio/local.h`, `stdio/fread.c`, `stdio/ungetc.c` and `stdio/makebuf.c`. Libm adds three modified files: the C-linkage wrapper in `fdlibm.h`, and `e_fmod.c`/`ef_fmod.c`. In both fmod implementations, the signed-zero array retains static `.sdata2` storage and `one` becomes an automatic constant. The numerical algorithm and values are unchanged; the complete functions and signed-zero tables match. These annotations reflect observed storage, not recovered historical spelling. All eight files are marked in the provenance inventory. These adaptations reproduce observed behavior rather than revising the C library's semantics. The modified compiler support header is separately inventoried.

## Verification and remaining work

`python3 tools/reconstruct.py` recompiles the accepted sources, verifies complete linked sections and function boundaries, and replaces each owned interval exactly once in the full analysis image. The resulting 2,860,576-byte DOL must match SHA-1 `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`; every allocated ELF byte, entry point and BSS extent is also checked. Generated data earns no code credit. Mutation tests cover source inclusion, constants, coverage and resolved small-data anchors for both compiler families.

The `add_separators` formatter helper and unaccepted math routines remain unresolved. In particular, powf is not credited: its candidate still differs in three indexed-load operand encodings. Runtime has not been tested. A matching analysis image is not yet a replacement game disc.

## SN formatting and math support

The [Resident Evil 4 decomp reference](https://github.com/adonis-singh/re4/tree/9b76ff003081d6ece8b838453fb8310743a972c7)
provides a reconstruction of the same SN formatting library. Its formatter and combined
math-support source were inspected at that pinned commit. [The provenance inventory](../config/GR8E69/sn-library-source.json)
records reference hashes; [NOTICE.SN](../src/newlib/NOTICE.SN) preserves attribution and the
reference project's license scope. Its tooling CC0 grant is not represented as a source
license. Newlib/Berkeley and Sun notices remain with their implementations.

| Variant / fragment | Code bytes | Functions | Generated data | BSS |
| --- | ---: | ---: | ---: | ---: |
| `newlib_vfprintf` | 8,104 | 7 | 256 | 292 |
| `newlib_vfiprintf` | 5,968 | 2 | 116 | 128 |
| `newlib_sn_math` | 2,324 | 4 | 224 | 0 |

The formatter is compiled as C with the existing libc profile, once normally and once
with `INTEGER_ONLY`. Rising Sun bounds string-stream copies by `FILE._w`, decrements
that capacity, and ends formatting when it is exhausted. The apostrophe flag selects
`add_separators` for decimal integers and floating output. That helper remains original
context. Both complete formatter bodies, the dispatch/helper functions, private buffers,
strings and constants match. `strround` and `itoa` retain their original global binding.

The two original file records both say `vfprintf.c`. Their symbol-table indices, 4840
and 4850, distinguish their private BSS. SN's sized `STT_NOTYPE` `.lcomm` records are
checked as storage records; zero-sized labels establish no ownership. As with MW static
suffixes, SN's compilation-local `.number` suffix does not replace the checks of base
identifier, binding, address, size and file scope. Mutation tests reject changed sizes,
identifiers, and the other same-named file record.

The four `sn_*` math helpers use Newlib 1.8.2's floor, fmod, log and log10 algorithms
in one source unit, consistent with `math_support.c` and its compiler marker. They use
`-O2 -G8 -fsigned-char` as C. Scalar coefficients become constant expressions because
separate unused scalar objects are absent from the target. Values and computations are
unchanged; the addressed `Zero` array retains static storage. Every generated byte of
the combined 224-byte constant block is verified. These annotations and build settings
are working reconstructions, not proof of the historical command line.
