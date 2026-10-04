# Dolphin SDK matching evidence

The accepted SDK contribution is **166,200 code bytes in 686 functions across 89 source units/fragments**. Together with the other accepted game and library fragments, the complete build verifies **329,936 / 2,492,032 executable bytes (13.239637%)**, across 1,277 functions. SDK code is reported as `restored_library`, separately from reconstructed game code. The coverage includes runtime libraries, not just gameplay logic.

The eleven additional [network-library fragments](Network.md), totaling 16,448 code
bytes, are tracked separately from the 89 SDK units below.

## Source and attribution

Sources and headers derive from [doldecomp/dolsdk2004](https://github.com/doldecomp/dolsdk2004/tree/2328b4164b1a98422a2255d83ce9a5a7548990cc), commit `2328b4164b1a98422a2255d83ce9a5a7548990cc`. That project reconstructs 2004 SDK libraries. The game's embedded release identifiers instead include April 17 and April 22, 2003 build dates and compiler marker `0x2301`. It is therefore a reference, not proof that the game used the 2004 release.

The older `__GXSetVAT` loop comes from [dolsdk2001/GXAttr.c](https://github.com/doldecomp/dolsdk2001/blob/eb1234c45e6df75757c652c835507ca89674f9a8/src/gx/GXAttr.c), with its `gx` reference renamed to `__GXData`. The pinned file SHA-256 is `24198fc8241505c87aaff62eb33542f90fc03e67db62cf58dab8b3a7a48ac180`.

The seven GBA units, OSFont, GXFrameBuf, dvdqueue, OSInterrupt, OSAlarm and the older ClearArena implementation additionally derive from [PrimeDecomp/prime](https://github.com/PrimeDecomp/prime/tree/693fa74f9a9a7cdbb97f09a09d30853415e40927). Its unmodified CC0 notice is preserved as `LICENSE.PrimeDecomp`. The target's DWARF independently establishes the GBA structures and recovered field names.

See [NOTICE](../src/dolphin/NOTICE) and [file provenance](../config/GR8E69/dolphin-source.json). Existing source comments are retained. No upstream license grant has been invented. Adaptation and verification used AI assistance; original source spelling and the exact compiler release remain unproven.

## Accepted coverage

| Library | Units | Functions | Matching code bytes |
| --- | ---: | ---: | ---: |
| AI | 2 | 18 | 2,188 |
| AMCSTUBS | 1 | 7 | 40 |
| AR | 2 | 15 | 8,028 |
| AX | 8 | 60 | 11,340 |
| CARD | 17 | 98 | 29,176 |
| DSP | 3 | 15 | 2,916 |
| DVD | 7 | 84 | 16,972 |
| EXI | 2 | 27 | 8,112 |
| GBA | 7 | 35 | 7,160 |
| GX | 13 | 153 | 32,132 |
| MTX | 1 | 2 | 360 |
| OS | 21 | 105 | 25,536 |
| PAD | 2 | 20 | 7,184 |
| SI | 2 | 26 | 6,504 |
| VI | 1 | 21 | 8,552 |

Each manifest records original function names, bindings, addresses, sizes, section ownership, dependencies and the compiler profile. Each accepted text interval contains precisely the original functions covering that interval. Unused library functions are stripped by the native linker and earn no credit. No assembly function bodies have been added to the accepted sources.

## Compiler and linker

The SDK is compiled with CodeWarrior `GC/1.2.5n` from the same checksum-pinned community compiler archive used for ProDG. The common profile is `-O4,p -inline auto -sym on -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn pragmas -requireprotos -pragma "cats off"`, with `__GEKKO__`, `RELEASE` and the reference tree's `SDK_REVISION=1` defined. That last macro selects reference-source conditionals; it is not a claim that the game uses the reference project's May 2004 revision. EXIBios uses its upstream `-O3,p` profile. DVD, OSFont and CARDOpen/CARDRename use signed `char`; the other accepted units use unsigned `char`.

OSInterrupt, ai_src, OSCache, GXInit, OSTime, OSDumpContext, OS and OSSync additionally use `-opt nopeep`. This reproduces the observed C code following low-level routines in the references; it is a verified build setting, not proof of why the historical build selected it. OSAlarm, ai_src, OSMemory and OSFatalScreen disable unused-function stripping to retain private C callbacks referenced by original context. Explicit inline C helpers prevent absent out-of-line copies without replacing any function body with assembly.

CodeWarrior emits relocatable objects directly. ProDG 3.8.1's linker resolves their relocations at pinned original addresses, using `--strip-unused` and `--fix-sda`. The original initialization and symbols establish `r13 = 0x80459220` and `r2 = 0x80469220`. The game places SDK `.sdata2` within the r13 bank; section names alone cannot select the register.

MW `R_PPC_EMB_SDA21` references need section-relative external definitions for this linker. `sdk_build.py` supplies zero-size input sections containing symbol labels only, at validated original addresses. These anchors contain no instructions or data. Explicit input selectors prevent them from being absorbed into a source unit's data. The linker removes empty anchor sections while retaining resolved symbol values, so the verifier checks those exact values separately. All final relocations must be gone, and all compiled code/data must match. Emitted instructions are never patched.

## Adaptations to the target

All source differences from the pinned 2004 reference are inventoried by the provenance manifest. The substantive changes are:

- AR, ARQ, AX, CARD, DVD, DSP, EXI, PAD, SI and VI retain the exact build identifiers embedded in the target; DSP's diagnostic date/time literals are also restored. AR omits an unused synthetic math include absent from its original constant layout.
- `math.h` uses automatic constant locals in inline square-root helpers. This preserves their computation while avoiding unused static constant objects absent from the game's GXPixel data. The full generated constant block is compared; unused header facilities and debug builds remain unverified.
- GX uses the older vertex-format loop, retains indexed array base/stride caches in release code, keeps the FIFO pointers file-local in original storage order, and clears the FIFO wrap bit using a field operation. Culling uses the original switch. Fog and TEV colors use individual byte fields rather than packed word loads.
- DVD file reads preserve the original strict `offset < length` check and the three embedded diagnostic line numbers. OS reset timing uses the original six-bit field. The no-change GBS SRAM path calls `__OSUnlockSram(FALSE)`, as shown by its original zero offset argument, rather than the later extended-SRAM wrapper.
- VI omits the later extra-timing API, its private storage and modes 28–30. It restores the original mode handling, TV-format register selection, separate clock selection, diagnostic line number and register-write order. Constants, jump tables, all 21 retained functions and all storage objects match.

- CARD uses the original fixed 128-byte writes and sector-erase behavior. The later fast-mode flag, vendor probe and variable page-size paths are absent from the retained code; the private fast-mode object is also absent from the original symbols. The write-loop increments and unsigned division match the original instructions.
- PAD omits the later barrel-controller detection and storage. Its retained single-channel motor function omits the later global rumble-disable check, and recalibration clears its flag with the original byte mask. All 18 retained functions and their data/storage match.
- AXVPB initializes the original `0xA4` synchronization flags. `AXGetLpfCoefs`, its mathematical include and associated constants are omitted: neither the function nor those constants exist in this unit's original layout. The retained ratio constant, cycle tables, voice buffers and all 13 functions match.
- DVD restores the fatal-error state assignment at the original call sites, including timeout paths, rather than unconditionally in the final error callback. The later command-16 motor-state paths and private flag are absent. The two retained panic line numbers are 644 and 2868. All 47 retained functions, jump tables, constants and storage match.

- EXIUart brackets its original lock/write/unlock sequence with interrupt disable/restore, including the failed-lock path. SISamplingRate restores the target's NTSC line/count pair 15/18. Padclamp omits absent circular-clamp APIs and their constants.
- GBA fields use the types preserved by DWARF, including unsigned byte counts and a void pointer; two private callback/helper bindings are restored. GBAKey's reference DSP microcode remains data and earns no executable-code credit.
- GXFrameBuf is a function-only fragment. Its display-mode objects remain original context because the four retained global mode symbols do not establish ownership of the intervening data. GXMisc restores the observed abort sequence. OSFont, OSLink, dvdqueue and fstload reproduce their complete retained functions and generated sections.

- OSInterrupt and OSAlarm retain only their C routines; the original interrupt-entry routines remain context. SetTimer is explicitly inline. File-local dependencies are resolved against the original file, binding and address, including the audio callback stack switch and five audio timing variables.
- Audio initialization restores its April 17, 2003 identifier and excludes later profiling globals. Its prefix and `__AI_SRC_INIT` are separate fragments around the original stack-switch routine. The retained C declarations account for every owned storage object, including state consumed by original context.
- GXInit restores its April 21, 2003 identifier, function-local shutdown/reset state, and the original register-pointer storage order. The revision-register setup is inside GXInit, and the later indirect-mask call is absent. GXLight preserves the observed mask/merge operations for RGB colors and light masks. Matrix coverage is limited to the two C projection routines and their constant pool.
- OS startup restores the older ClearArena saved-region behavior, April 17, 2003 identifier, storage bindings and declaration order. The floating-point zero objects are preserved for original context. OSDumpContext inlines the reference C context getter/clearer while leaving low-level context switching external. Reset callbacks stop at the first failure before synchronizing SRAM. Memory protection, fatal-screen rendering, cache, time conversion and system-call initialization are explicitly scoped C fragments.

These changes recover the observed older behavior; they are not proposed fixes for the SDK. No claim is made that unretained functions or debug configurations reproduce this game's SDK.

## Verification and limits

Private context dependencies require an explicit original-file scope, local binding and exact original symbol address. A same-named symbol elsewhere cannot satisfy the reference.

The verifier rejects unexpected compiler functions, unaccounted allocated sections, partial functions, unresolved runtime dependencies and overlapping ownership. File-local discarded helpers are scoped to the original source-file symbol; a same-named local in another SDK file does not make them the same function. Compiler-generated numeric suffixes on local static data names are ignored only while comparing their underlying C identifier, binding, exact address and size.

BSS needs more than a zero-byte comparison: every named object in each claimed BSS range must match the original file scope, address, size and binding. Partial objects and foreign storage are rejected. This caught unresolved ownership of `AXOut`'s profiling buffer: the reference declaration overlaps the following original storage. The accepted AXOut fragment excludes `__AXOutNewFrame` and that declaration, leaving the 440-byte function as original context. Its seven retained functions and four nonoverlapping buffers match.

The final image includes each verified compiler-produced interval exactly once and removes its corresponding original interval. Source BSS reservations are included with linker address/size assertions. The complete 2,860,576-byte derived DOL equals the pinned reference SHA-1 `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. Every original allocated file byte, entry point and BSS extent is also checked independently. Across the entire project, 30,454 generated data bytes and 116,298 BSS bytes are verified but earn no code credit.

Run `python3 tools/reconstruct.py` and `python3 -m unittest discover -s tests -v`. The verification tests include corrupted code/data, missing BSS endpoints, wrong progress denominators, duplicate coverage, incorrect BSS object sizes, changed SDA anchors a discarded local helper changed to global binding, and incorrect private-dependency scope, binding or address. Build reports fingerprint source, headers, manifests and tool binaries.

Runtime has not been tested. The verified image is an ELF-derived analysis DOL, not a replacement disc. Public progress uses the source-pinned verification snapshot described in [Progress.md](Progress.md).
