# MathFun.cpp: first reconstructed fragment

Six functions, **336 code bytes**, are compiled from `src/MathFun.cpp`. These are a partial reconstruction of one original file, not six complete source units. The project also restores 18 [Lua units](Lua.md); the remaining bytes stay original context. AI assistance was used for analysis, reconstruction and verification tooling.

## Evidence and boundaries

The pinned `MOH3RDVD.ELF` symbol table contains a `MathFun.cpp` file entry and an associated local `gcc2_compiled.` marker at `0x80133590`. The next file is `collision.cpp`, whose marker is at `0x80133bd4`. Named MathFun functions occupy this 1,604-byte interval. The accepted fragment is the contiguous interval `[0x80133904, 0x80133a54)`; ten other functions in the original interval remain unreconstructed. No class layouts or vector types are needed for this fragment.

| Original symbol | Address | Bytes |
| --- | --- | ---: |
| `MathFunCloseToZero__Fff` | `0x80133904` | 40 |
| `MathFunSRandom__FUi` | `0x8013392c` | 44 |
| `MathFunRandomUI32Raw__Fv` | `0x80133958` | 32 |
| `MathFunRandomSign__Fv` | `0x80133978` | 40 |
| `MathFunRandomSignOrZero__Fv` | `0x801339a0` | 60 |
| `MathFunRandomI64__Fxx` | `0x801339dc` | 120 |

The ELF names and sizes are checked on every build against `config/GR8E69/MathFun.json`. Calls resolve to original `srand` at `0x80265c8c` and `rand` at `0x80265c98`. The original four-byte `g_bMathFunRandomSeeded` object is at `0x802c4d40`; it remains in the retained original data. The source declares it externally and writes the observed four-byte value 1.

## Compiler profile and uncertainty

The working profile is the archive's **ProDG 3.8.1**, `ngccc -O2 -G0`. Its diagnostics identify GNU C++ 2.95.2 SN BUILD v1.55 and NGCCC v1.2.1.112. Flags apply to the entire reconstructed fragment; there are no per-function options, instruction patches or handwritten assembly implementations.

During investigation, all five available ProDG candidates (3.5, 3.5b140, 3.7, 3.8.1 and 3.9.3) produced identical linked bytes for these six functions with this source and these flags. **This does not identify the original compiler release or its complete command line.** Additional, larger functions are needed to distinguish candidates. `-G0` is consistent with the observed full-address reference to the seeded flag, but is not uniquely established by that reference.

The compiler archive and wibo runtime are pinned in `tools/compilers.json`. [wibo 1.0.3](https://github.com/decompals/wibo/releases/tag/1.0.3) runs the Windows tools on this macOS ARM64 host using its existing Rosetta support. Nothing is installed system-wide. The lock format and artifact checksums were obtained from the public [NFL Street 2 compiler lock](https://github.com/mitsevox/nflstreet2/blob/main/tools/compiler-tools.json); this is tooling provenance, not evidence that the games used the same compiler profile.

## Source fidelity review

- Function names and argument encodings are recovered from the ELF. `ff` denotes two floats, `Ui` unsigned int, and `xx` two signed long longs. Parameter names in the reconstructed source are descriptive names, not recovered names.
- Return types are inferred from instructions and ABI use; GCC's ordinary function mangling does not encode them. In particular, the original boolean typedef or spelling for `MathFunCloseToZero` and the seeded flag is unknown. The chosen declarations preserve the observed register result and four-byte flag store.
- `MathFunCloseToZero` preserves the two floating comparisons and negation. The source does not replace them with an absolute-value approximation or add special handling for NaNs, signed zero or negative tolerances.
- `MathFunRandomSign` uses bit 1 of `rand()`, then subtracts 1. `MathFunRandomSignOrZero` uses a signed 64-bit product by 3 and a right shift by 31. Neither has been replaced by a modulo operation or a different generator.
- `MathFunRandomI64` computes the inclusive range before calling `rand`, multiplies in 64 bits, shifts arithmetically by 31, and adds the lower bound. Signed overflow and negative right-shift behavior are not generalized into a portable API contract; this work verifies the target compiler's exact emitted instructions. Original typedef names and intended input limits remain unknown.
- No generated data sections are present in this fragment. The build rejects additional allocated output instead of discarding it. Source coverage includes the complete generated text section, all six function bodies and their resolved references.

## Complete-image build

Run `python3 tools/reconstruct.py`. It builds all accepted units, starting a fresh `build/reconstruction/` so a failed build cannot leave an old success report. MathFun's intermediate files are under `build/reconstruction/units/MathFun/`.

1. Validate the target executable and all function/external-symbol boundaries.
2. Compile the source through the SN driver to assembly, then run the pinned SN assembler. The unmodified compiler assembly is only an intermediate output. Separating these normal stages works around the driver returning success without an object under wibo.
3. Link the unmodified object with SN's linker at its original address. GNU ld rejects SN's object symbol-table ordering; SN's linker consumes it directly. Check the resulting section size, all function addresses/sizes, external addresses, absence of unresolved symbols or remaining relocations, and every compiled byte against the original fragment.
4. Wrap the entire prelinked text section and the remaining original ranges in a context object. Original bytes in the replacement interval are excluded. The generated `.incbin` directives package bytes; none substitutes for C++ implementation. GNU ld places the context at the original section addresses and retains the empty `.sbss2` endpoint, which SN's full-image linker drops. Assertions fix both ends of the source interval. No emitted source instruction is modified.
5. Compare the **entire 2,860,576-byte ELF-derived DOL**, header and padding included. SHA-1 must be `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. Also check each original allocated ELF byte, the entry point and all BSS extents. Write a report only after all checks pass.

This is a partially source-built, fully compared analysis image. It is not a reproduction of the original ELF's debug tables or an emulator-tested replacement disc. The intermediate prelink/context packaging is a bootstrap approach for this fixed-address fragment; a larger project will need reviewed source/data splits and a broader native linking configuration.

Mutation tests reject changed instructions, an incorrect external, incomplete function coverage, and unexpected generated allocated data. An integration test also corrupts the generated code blob and relinks the complete image, confirming that retained original bytes cannot conceal a bad source result.
