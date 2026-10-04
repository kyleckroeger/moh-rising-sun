# STLport: verified tree helpers and container instantiations

**192 functions compile to 71,452 matching code bytes**: six
`_STL::_Rb_global<bool>` helpers (1,764 bytes) and 186 container/algorithm instantiations
(69,688 bytes). The helpers provide shared red-black-tree rotation, rebalancing
and iterator traversal.
This establishes a compatible STLport implementation for this subset, not the
exact historical release or compatibility of every container. AI assistance was
used for source integration, compiler experiments and verification.

## Reference and license

The reference is the public [STLport 4.5.3 archive](https://sourceforge.net/projects/stlport/files/STLport%20archive/STLport%204/).
[stlport-source.json](../config/GR8E69/stlport-source.json) pins the archive's SHA-256
and the three upstream files. `src/stlport/tree.h`, `tree_impl.h` and `swap.h` are
focused excerpts of `stlport/stl/_tree.h`, `_tree.c` and `_algobase.h`.
Each retains its upstream copyright/permission notice and marks the modification.
The permission allows copying/modification with those notices retained.

The excerpts keep the upstream node fields, member declarations, template
parameter, algorithms and swap operation. Unrelated container templates and
configuration/includes are omitted; `_STLP_CALL` expands to nothing and
`_STLP_STD` to `_STL`. The small `.cpp` files are explicit instantiations written
for this project, not recovered original translation-unit boundaries. No assembly
implementations or instruction patches are used.

## Compared ranges

| `_Rb_global<bool>` method | Original address | Code bytes |
| --- | --- | ---: |
| `_Rotate_left` | `0x80279a9c` | 96 |
| `_Rotate_right` | `0x80279afc` | 96 |
| `_Rebalance` | `0x80279b5c` | 360 |
| `_M_decrement` | `0x80279de8` | 112 |
| `_M_increment` | `0x8027a1dc` | 88 |
| `_Rebalance_for_erase` | `0x8027de58` | 1,012 |

The working profile is ProDG 3.8.1, `-O2 -G0 -fno-implicit-templates`. Explicit
instantiation selects one original function per build fragment; implicit
out-of-line definitions of other methods are suppressed. Each entire generated
object has only its expected `.text` section. The rebalance methods link their
rotation calls to the original named addresses, now also rebuilt from source.
All symbols, sizes, bindings, references and complete text bytes are checked.
No generated data is discarded, and the complete analysis-image comparison is
required. These results do not establish the original compiler release.

The inherited upstream node layout uses a boolean color and three pointers.
The compiler's byte load/store for the color and pointer accesses at offsets 4,
8 and 12 agree with the original. No game class layout is needed. The namespace,
template argument `bool`, method names and argument encodings are present in the
original mangled symbols; return types and layout are supported by the reference
and instruction comparison.

The initial [dependency scan](Dependencies.md) found **137 distinct unfinished caller
ranges** across these six helpers, counted once across the set. This measures
research reach, not additional reconstructed code or a claim that those callers
will match automatically.

## Container instantiations

The initial container profile added the following complete function bodies:

| Family | Functions | Code bytes |
| --- | ---: | ---: |
| Tree maps and sets | 76 | 29,736 |
| Deque helpers | 6 | 1,056 |
| Vector reserve/assignment | 4 | 1,020 |
| Total added | 86 | 31,812 |

Each file in `src/stlport/containers/` explicitly instantiates one original
symbol. Address-based filenames are project identifiers, not recovered original
source paths. The corresponding manifest records its complete mangled type,
original address, binding, size, generated sections and external references.
Class typedefs are local readability aliases and do not change mangling.

Those first 86 instances use builtin types, STLport strings/pairs, and pointers to
game objects. All game pointees are forward-declared. No game class is sized,
constructed, copied by value or given speculative fields. The upstream container,
allocator, node and string declarations supply library layouts; matches across
insertion, erasure, allocation, copies and traversal independently test their
observed use. Custom comparators and unknown game value layouts were outside
that initial subset. The scoped extension below adds reviewed game storage models.

`include/stlport-containers/` now contains 72 **unmodified** upstream files selected
by compiler dependency scans across the accepted instantiations. Their individual
hashes are recorded in `stlport-source.json`, along with the existing archive
hash. Copyright/permission notices remain in every file. Unused declarations and
other build configurations in these headers are not independently verified.

The project profile uses `_STL`, new/delete allocation, no exceptions, no
threading, no iostreams and no wide-character facilities. Native C includes use
the existing target Newlib headers. Compilation uses ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. These are supported settings for
the retained subset, not proof of the historical command line or full C++ runtime.

### Native allocation declarations and the earlier string blocker

The earlier `vector<void*>::reserve` experiment produced 232 matching code bytes
plus an unplaced `bad_alloc` string from the GCC native `new` header's unused
inline `bad_alloc::what` definition. That generated object remains unaccepted;
no arbitrary original string copy was chosen and no generated bytes were cut out.

The integration header now keeps `what` as an **out-of-line declaration**, while
retaining the class shape, virtual declaration and allocation signatures. No
accepted instantiation calls or constructs this exception. This is a deliberately
narrow declaration boundary for unused runtime support, not a claim to have
recovered the game's native `new` header. The GCC 2.95.2 `exception` header is
unchanged; copyrights, source hashes and URLs are preserved, and GNU GPL v2 is
included as `COPYING.GCC`. See [CONTAINERS_NOTICE](../src/stlport/CONTAINERS_NOTICE).

All 86 resulting objects contain exactly their one expected text function and
**no generated allocated data**. The original pointer-vector routine is now
accepted in that profile. No linker discard, byte patch, assembly replacement or
masked comparison is involved. New/delete calls resolve to the previously
verified game allocation operators; other dependencies resolve to original
symbols and may still be original context.

### Scoped game-container extension

A further 44 functions add 16,592 bytes using recovered comparator behavior and
observed game-value storage bounds. These include tree maps, vector reserve and
pathfinding-link list operations. Unknown ranges remain unsigned-byte storage;
nontrivial game copy/destructor calls remain original context. The separate
256-byte pathfinding copy constructor is counted as reconstructed game code.
See [GameContainers.md](GameContainers.md) for per-family counts, original
instruction evidence, scoped declarations and limits. These storage models are
not complete gameplay class definitions.

### Sorting, heap, and resource extension

A further 56 functions add 21,284 bytes across scoreboard/leaderboard sorting,
render-command heaps, target selection, managed resources, and pointer helpers.
Target sorting requires a verified fast-math profile and nine complete constant
pools. Its nine weak-pointer event handlers are counted separately as game code.
See [GameAlgorithms.md](GameAlgorithms.md) for declarations, evidence, compiler
settings and per-family counts.

### Validation and next work

The ordinary source build checks every compiled symbol and byte, then includes
all verified source intervals in the complete-image comparison. Existing input
fingerprints cover the configuration, native declarations, upstream files and
all instantiations. A future template that needs unavailable exception support
must not be treated as supported by these results.

Remaining STL-named symbols include other custom comparators, by-value game
objects, algorithms over game structures and some string support. Each still
requires independent type/behavior evidence beyond the scoped models now accepted. A symbol mentioning STL is not automatically
reusable library code. Continue with complete generated-output comparisons and
retain unresolved candidates under `scratch/`.
