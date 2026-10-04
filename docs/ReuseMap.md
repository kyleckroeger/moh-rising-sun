# Reuse index and next work

Use the code map to find candidates, then use shared-library references and dependency
evidence to choose work. A symbol-name match is a research lead; only complete generated
code/data and full-image verification establish accepted source coverage.

## References that have already produced matches

| Family | Reference | Accepted code bytes | Practical lesson |
| --- | --- | ---: | --- |
| Dolphin SDK | [dolsdk2004](https://github.com/doldecomp/dolsdk2004/tree/2328b4164b1a98422a2255d83ce9a5a7548990cc), [Prime](https://github.com/PrimeDecomp/prime/tree/693fa74f9a9a7cdbb97f09a09d30853415e40927), older GX reference | 166,200 | Recover the game's older revision and storage layout; a later SDK is a reference, not a drop-in replacement. |
| Lua | [Lua 4.0.1](https://www.lua.org/ftp/lua-4.0.1.tar.gz) | 51,640 | Shared headers, allocator integration and constant placement unlock whole units. |
| Newlib | [Newlib 1.8.2](https://www.sourceware.org/ftp/newlib/newlib-1.8.2.tar.gz) | 37,496 | Correct FILE and varargs declarations unlock many libc routines. |
| SN library adaptations | [RE4 formatter and math support](https://github.com/adonis-singh/re4/tree/9b76ff003081d6ece8b838453fb8310743a972c7/src/game) plus Newlib | 16,396 | Rising Sun's bounded string writes and grouping flag distinguish its formatter. |
| GCC runtime | [GCC 2.95.2](https://github.com/gcc-mirror/gcc/tree/releases/gcc-2.95.2/gcc) | 5,532 | Target ABI, configuration and private tables matter as much as function names. |
| STLport | [STLport 4.5.3](https://sourceforge.net/projects/stlport/files/STLport%20archive/STLport%204/) | 33,576 | Six tree helpers and 86 container instances match using one reviewed profile; custom game value types and the exact historical release remain unconfirmed. |
| Network helpers and RSA MD5 | [SocketLibrary](https://github.com/Cuyler36/dolsocketlibrary/tree/e02356692695e64429dd70dc421a4a71a493e9c8), [NetworkBasePackage](https://github.com/Cuyler36/dolnetbp/tree/ba3846a83bf86505fe60e07aefe89e3e0ffc0e34) | 16,448 | Routing, DNS/DHCP diagnostics and packet handling now join the retained helpers; broader stack versions differ. The digest source is the RSA Data Security, Inc. MD5 Message-Digest Algorithm. |

These are separate, already accepted byte totals, not estimates of future gains.
See the individual library evidence documents for exact scopes, provenance and licenses.
The 25,296-byte increase from 10.000995% to 11.016070% combines three Lua units and the
SN library adaptations. It does not represent an equivalent amount of gameplay logic.
The next pass adds 1,764 STL helper bytes and 192 game allocation-operator bytes,
bringing coverage to 11.094561%. The following CMatrix pass adds 2,120 game-code
bytes across 16 functions, bringing coverage to 11.179632%; see [matrix layout
and verification evidence](Matrix.md). See [STLport](STLport.md), [allocation contracts](Memory.md)
and the [direct dependency map](Dependencies.md). The current pass adds 40,720
bytes from the remaining Lua loader, network helpers and container instances,
reaching 12.813640%. The older network-library pass adds another 10,616 bytes,
reaching 13.239637%. The active target is 15%; at least 43,872 more matching bytes
are needed. This is a checkpoint, not completion of that target.

## Ranked research queue

This is a qualitative work order, not a prediction of hours or a promise of matches.

| Priority | Candidate | Evidence / size | Next useful step |
| --- | --- | --- | --- |
| 1 | Remaining STL instances | 122 remaining symbols mention `_STL`, spanning 43,668 declared function bytes | The builtin/pointer container subset now matches. Recover custom comparators and by-value game structures before instantiating further types. This includes game routines, not just reusable library code. |
| 2 | `powf` | 1,780 bytes; the research candidate differs in three indexed-load operand encodings | Investigate front-end/source address-expression ordering; do not patch instructions. |
| 3 | SN `add_separators` | 392 bytes; its ABI and formatter callers now match | Finish the helper's C reconstruction and register allocation; it remains context. |
| 4 | Older network stack | The six accepted helper units establish a useful reference, but larger TCP/UDP/PPP and Ethernet routines differ | Compare protocol structures and behavior with original call sites, keeping retained code and storage complete. |
| 5 | Shared game math, strings and allocation | `MathFun.cpp` has 1,268 unmatched bytes; string/allocation groups are also mapped | Recover common types, allocator contracts and constant ownership, then work through callers. |

The verified STL container profile now covers builtin, string and pointer
instances. The exact original release and broader configurations remain
unconfirmed. Do not invent game class layouts to instantiate further templates.
[NFL Street 2](https://github.com/mitsevox/nflstreet2) is a useful existing compiler-tooling
reference; shared EA game/engine source compatibility has not been established here.

## Mapping work with the highest payoff

The direct-call portion of item 2 is now implemented by `tools/dependencies.py`.
Its [ranked work packets](Dependencies.md) selected CMatrix for shared game-type
work: initialization had 173 distinct unfinished callers. Initialization,
assignment, multiplication and 13 related functions now match using one shared
header. CVector3 remains opaque pending evidence of its complete layout.
Vtable/data references and broader type recovery remain future work.

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
