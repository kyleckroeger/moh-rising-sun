# Reuse index and next work

Use the code map to find candidates, then use shared-library references and dependency
evidence to choose work. A symbol-name match is a research lead; only complete generated
code/data and full-image verification establish accepted source coverage.

## References that have already produced matches

| Family | Reference | Accepted code bytes | Practical lesson |
| --- | --- | ---: | --- |
| Dolphin SDK | [dolsdk2004](https://github.com/doldecomp/dolsdk2004/tree/2328b4164b1a98422a2255d83ce9a5a7548990cc), [Prime](https://github.com/PrimeDecomp/prime/tree/693fa74f9a9a7cdbb97f09a09d30853415e40927), older GX reference | 166,200 | Recover the game's older revision and storage layout; a later SDK is a reference, not a drop-in replacement. |
| Lua | [Lua 4.0.1](https://www.lua.org/ftp/lua-4.0.1.tar.gz) | 48,564 | Shared headers, allocator integration and constant placement unlock whole units. |
| Newlib | [Newlib 1.8.2](https://www.sourceware.org/ftp/newlib/newlib-1.8.2.tar.gz) | 37,496 | Correct FILE and varargs declarations unlock many libc routines. |
| SN library adaptations | [RE4 formatter and math support](https://github.com/adonis-singh/re4/tree/9b76ff003081d6ece8b838453fb8310743a972c7/src/game) plus Newlib | 16,396 | Rising Sun's bounded string writes and grouping flag distinguish its formatter. |
| GCC runtime | [GCC 2.95.2](https://github.com/gcc-mirror/gcc/tree/releases/gcc-2.95.2/gcc) | 5,532 | Target ABI, configuration and private tables matter as much as function names. |

These are separate, already accepted byte totals, not estimates of future gains.
See the individual library evidence documents for exact scopes, provenance and licenses.
The 25,296-byte increase from 10.000995% to 11.016070% combines three Lua units and the
SN library adaptations. It does not represent an equivalent amount of gameplay logic.

## Ranked research queue

This is a qualitative work order, not a prediction of hours or a promise of matches.

| Priority | Candidate | Evidence / size | Next useful step |
| --- | --- | --- | --- |
| 1 | Remaining Lua `lundump.c` | 3,076 bytes; Lua 4.0.1 candidate has matching retained function sizes | Resolve strings/constants and allocator-string placement using the lessons from the three accepted prefix layouts. |
| 2 | `powf` | 1,780 bytes; the research candidate differs in three indexed-load operand encodings | Investigate front-end/source address-expression ordering; do not patch instructions. |
| 3 | SN `add_separators` | 392 bytes; its ABI and formatter callers now match | Finish the helper's C reconstruction and register allocation; it remains context. |
| 4 | MD5 | The map's local-symbol group spans 3,224 bytes | Identify the implementation and its endian helpers before assuming an RFC implementation will match. File evidence is local-only. |
| 5 | STL-family templates | 214 remaining symbols mention `_STL`, spanning 77,244 declared function bytes | Identify the exact header family/version and ABI; begin with pointer/integer containers and common tree routines. This footprint includes game functions whose signatures mention STL types. It is **not** 77,244 verified reusable library bytes. |
| 6 | Shared game math, strings and allocation | `MathFun.cpp` has 1,268 unmatched bytes; string/allocation groups are also mapped | Recover common types, allocator contracts and constant ownership, then work through callers. |

The STL lead is potentially larger than individual leaf-function work, but no compatible
header release has been verified. Do not invent class layouts to instantiate templates.
[NFL Street 2](https://github.com/mitsevox/nflstreet2) is a useful existing compiler-tooling
reference; shared EA game/engine source compatibility has not been established here.

## Mapping work with the highest payoff

1. **Reference/version inventory.** For each family, record source URL and pinned commit,
   license, symbol/constant evidence, compiler profile, compared ranges, and unresolved
   differences. Keep candidate, partial research match, and accepted source distinct.
2. **Dependency and type map.** Record direct calls, data references, vtables, and shared
   structures. Rank helpers by how many unfinished callers depend on their declarations.
   A direct-call scan misses indirect calls and virtual dispatch; label those gaps.
3. **Reusable comparison batches.** Once a header/compiler profile is supported, compile
   a coherent library family and retain per-function *and* whole-section results. This
   finds common blockers without accepting isolated green functions over bad data.
4. **Contributor work packets.** Link a grey group to its candidate source, byte count,
   precise blocker and verification command. Small, reproducible packets reduce duplicate
   research and make AI-assisted proposals easier to review.

The target's preserved DWARF covers seven GBA-library units, not all game classes.
Game types and file ownership still need evidence. Keep original bytes, full debug
exports and local paths out of public reports. None of these maps earns code credit.
