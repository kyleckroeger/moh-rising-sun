# Lua source restoration

18 Lua compilation units restore **48,564 executable code bytes in 292 functions**, plus **4,032 bytes of read-only data**. Every retained function and every generated data byte matches the pinned `MOH3RDVD.ELF`. They are integrated into the complete-image comparison run by `python3 tools/reconstruct.py`.

This is restoration of an existing open-source library, with its game integration recovered from the executable. It is reported separately from independently reconstructed game-specific code. AI assistance was used to identify the source version, investigate the allocator/linker behavior, configure the build and verify the results; the original Lua implementation was written by its credited authors.

## Source and license

The source is [Lua 4.0.1](https://www.lua.org/ftp/lua-4.0.1.tar.gz), from the [official source archive](https://www.lua.org/ftp/). Archive SHA-256:

`df746e149cf6939e90009d2e540eee918d585b4d1bc6d68b19316a050d484d2a`

The 18 C files and supporting Lua headers are in `src/lua/`. The original copyright and permission notice is preserved in `src/lua/COPYRIGHT` and in the source headers. All vendored Lua files are unmodified upstream files except `lmem.h`, `lmem.c`, `ldo.c`, `lapi.c`, `lcode.c` and `ltable.c`, which are explicitly marked as modified. The archive is only needed to audit provenance; builds use the checked-in source files.

Lua 4.0 was also investigated. Its `lparser.c` lacks the semicolon check in `retstat`, producing a 100-byte function where Rising Sun has 112 bytes. Lua 4.0.1 contains that check and reproduces the entire 9,704-byte parser. This evidence supports the selected source version for the accepted units; it does not claim every Lua unit in the game has been recovered.

## Accepted units

| Unit | Text interval, end exclusive | Functions | Code bytes | Generated read-only data |
| --- | --- | ---: | ---: | --- |
| `ldebug.c` | `0x8003a6d8–0x8003b2d0` | 21 | 3,064 | `0x80295f48`, 256 bytes |
| `lfunc.c` | `0x8003bc60–0x8003bf34` | 7 | 724 | `0x802960b0`, 4 bytes |
| `lgc.c` | `0x8003bf34–0x8003cac0` | 21 | 2,956 | None |
| `lparser.c` | `0x8003f0d8–0x800416c0` | 65 | 9,704 | `0x80296610`, 700 bytes |
| `lstring.c` | `0x80041978–0x80041d80` | 8 | 1,032 | `0x802968fc`, 4 bytes |
| `ltm.c` | `0x800426ec–0x80042a90` | 8 | 932 | `0x80296990`, 584 bytes |
| `lvm.c` | `0x80043694–0x800453a4` | 18 | 7,440 | `0x80296e10`, 296 bytes |
| `lauxlib.c` | `0x80039080–0x8003930c` | 5 | 652 | `0x80295d08`, 120 bytes |
| `ldo.c` | `0x8003b2d0–0x8003bc60` | 20 | 2,448 | `0x80296048`, 104 bytes |
| `llex.c` | `0x8003cac0–0x8003e21c` | 13 | 5,980 | `0x802960b4`, 600 bytes |
| `lmathlib.c` | `0x8003e21c–0x8003ec08` | 25 | 2,540 | `0x80296310`, 488 bytes |
| `lmem.c` | `0x8003ec08–0x8003ed2c` | 2 | 292 | `0x802964f8`, 44 bytes |
| `lobject.c` | `0x8003ed2c–0x8003f0d8` | 6 | 940 | `0x80296568`, 168 bytes |
| `lstate.c` | `0x800416c0–0x80041978` | 4 | 696 | `0x802968cc`, 48 bytes |
| `lzio.c` | `0x800453a4–0x800454ac` | 3 | 264 | None |
| `lapi.c` | `0x80038af4–0x80039080` | 17 | 1,420 | `0x80295c7c`, 12 bytes; `0x80295c88`, 128 bytes |
| `lcode.c` | `0x8003930c–0x8003a6d8` | 33 | 5,068 | `0x80295dbc`, 88 bytes; `0x80295e18`, 304 bytes |
| `ltable.c` | `0x80041d80–0x800426ec` | 16 | 2,412 | `0x8029693c`, 44 bytes; `0x80296968`, 40 bytes |

The original file symbols and `gcc2_compiled.` markers support these text-unit boundaries. Each manifest in `config/GR8E69/` enumerates the original function names, binding, addresses, lengths, external references and generated section placement. The build checks these against the pinned executable on every run. Read-only data addresses were derived from original references and then verified by linking and comparing the complete generated sections, including strings, constants and pointer tables.

## Recovered game integration

`lmem.h` routes allocation through `DWI_alloc("lua", size, 1024)` and frees through `DWI_free(pointer)`. These are the calls observed in the game, replacing upstream Lua's default `luaM_realloc` allocation/free macros. `lmem.c` restores `luaM_growaux` and `luaM_realloc`; reallocation uses `DWI_realloc("lua", pointer, size, 1024)`. The original meanings of the DWI flag and parameter typedefs are not asserted; the declarations preserve the observed 32-bit C ABI.

`include/target/` contains minimal declarations for the target C library. They supply the 32-bit `size_t`/`ptrdiff_t`, integer limits and the external function prototypes needed to compile these units. The stdio wrapper uses the verified target Newlib `FILE` and reentrancy declarations; see [Newlib evidence](Newlib.md). No host C-library implementation is substituted for target code.

`ldo.c` preserves the target's changed error handling. `luaD_breakrun` reports `LUA is screwed.\n`; `luaD_runprotected` links and initializes the original error record, calls its callback directly, restores the previous record and returns its status. The upstream setjmp/longjmp recovery path is absent in the observed code. The original `lua_longjmp` structure accounts for its stack storage; no synthetic padding is introduced.

## Observed constant placement

Three additional units reproduce an original string prefix followed by an independently
aligned constant block. The original instruction references establish both addresses;
the complete strings, tables and floating constants are compared. Ordinary assembly
of a single `.rodata` input would instead apply its maximum alignment to the prefix,
shifting some string addresses by four bytes.

`lapi.c`, `lcode.c` and `ltable.c` therefore give the affected string arrays explicit
`.rodata.prefix` placement and four-byte alignment. These are target storage annotations,
not claims about historical source spelling or original linker input sections. Their
array names are descriptive. The linker places both complete compiler-produced blocks
within the original `.rodata`; all gaps remain original context. No emitted instruction,
constant or object byte is edited. The algorithms retain their upstream source.

The unused `lua_ident` definition and its version-string object are absent from the
pinned executable and are omitted from `lapi.c`. Lua notices remain in the source and
COPYRIGHT. The Lua 4.0.1 parser evidence still applies; this is not a claim that the game
shipped an entirely unmodified Lua release. `lundump.c` remains unaccepted.

## Compiler and linker evidence

The accepted library profile is ProDG 3.8.1 from the pinned compiler archive, with **`-O2 -G0 -ffast-math`**, applied uniformly to all 18 Lua units. The working compiler is GNU C 2.95.2 with SN modifications. The floating-point option is supported by comparing the emitted branches and arithmetic with the target; it is not applied to the separate MathFun game-code profile. This is a verified working profile, not proof of the original toolchain release or exact historical command line.

The original executable lacks several functions present in upstream source, while retaining their associated constant data. SN's native **`--strip-unused`** behavior reproduces this. The source keeps the upstream functions; it does not delete or stub them. The link roots are the unit's exported functions actually present in the original ELF. Examples of removed functions from the initial seven units are listed below; every accepted manifest enumerates its complete discard list:

- `ldebug.c`: `getluaproto`, `lua_getlocal`, `lua_setcallhook`, `lua_setlinehook`, `lua_setlocal`.
- `lstring.c`: `luaS_createudata`, `luaS_newudata`.
- `ltm.c`: `luaT_realtag`, `lua_copytagmethods`, `lua_gettagmethod`, `lua_newtag`.

No discarded function contributes to progress. The build rejects a discard entry whose symbol exists in the original target. All retained functions, their sizes, binding and addresses must match the original unit exactly.

`ldebug.c`'s removed APIs reference `luaA_pushobject`, which is absent from the original executable. SN leaves its unused declaration in the output symbol table. The verifier checks every object relocation to it: all must originate in the explicitly discarded functions, never in retained code or data. The linked output must have no remaining relocations; this unused declaration is the only permitted unresolved name for this unit. The complete loaded-byte comparison then independently checks the retained result.

## Verification and limits

Compilation produces a fresh object for each unit. The build validates all generated allocated sections and accounts for all object functions before linking. After the native SN link, it compares every retained generated code/data byte against the original. The context build includes these complete verified sections at their original addresses and excludes the corresponding original ranges. GNU ld preserves the original empty BSS endpoint. No compiler instructions are edited, masked or replaced by assembly.

The final check compares the entire **2,860,576-byte ELF-derived DOL**, including header and padding, with SHA-1 `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. It separately checks every original allocated ELF byte, entry point and BSS extent. Source and tool fingerprints are recorded in `build/reconstruction/report.json` only after the complete build succeeds.

Tests corrupt compiled code and constants, inject an absent dependency into retained code, introduce overlapping credit, shrink the progress denominator, and relink corrupted generated blobs. These failures must all be detected. Runtime/emulator behavior and the original ELF's nonloaded debug tables remain outside this verification claim.
