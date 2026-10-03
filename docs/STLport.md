# STLport: verified tree helpers and container research

Six `_STL::_Rb_global<bool>` helpers now compile to **1,764 matching code bytes**.
They provide shared red-black-tree rotation, rebalancing and iterator traversal.
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

The [dependency scan](Dependencies.md) finds **137 distinct unfinished caller
ranges** across these six helpers, counted once across the set. This measures
research reach, not additional reconstructed code or a claim that those callers
will match automatically.

## Next container work packet

`_STL::vector<void*, allocator<void*> >::reserve(unsigned int)` at `0x8027aefc`
has **232 matching code bytes in a research build**, but is **not accepted**.
Its complete object emits an additional 12-byte `.rodata` block containing
`bad_alloc` and padding. Placement/ownership of that block has not been established.
The original contains many copies; choosing an arbitrary matching copy would not
prove ownership. Keep this candidate outside the source manifests until the
complete generated output is accounted for.

Reproduction ingredients:

- Unmodified STLport 4.5.3 `<vector>` and its dependencies.
- The project's Newlib and compiler C headers, plus GCC 2.95.2's
  [`new`](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95.2/gcc/cp/inc/new)
  and [`exception`](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95.2/gcc/cp/inc/exception)
  runtime headers. These are research references, not vendored accepted files.
- `_STLP_NO_IOSTREAMS=1`, `_STLP_USE_NEWALLOC=1`, `_STLP_NO_THREADS=1`; route native
  C includes to the project C headers and native runtime includes to those GCC
  headers; use the C-header wrapper mode rather than native C++ C headers.
- ProDG 3.8.1: `-O2 -G0 -fno-exceptions -fno-implicit-templates` and an explicit
  instantiation of that `reserve` method.
- Resolve `__builtin_new`, `__builtin_delete` and `memmove` to their original
  symbols. The first two are now reconstructed in [sys_memory.cpp](Memory.md).

This result supports the pointer-container path and allocator configuration for
the candidate. It does not validate other containers, exception behavior, or
game-specific value types. Preserve unknown class layouts for by-value elements.

Run `python3 tools/reconstruct.py` and the validation suite before accepting any
extension. The live progress includes only the six verified helpers above.
