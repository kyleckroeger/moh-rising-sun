# CMatrix reconstruction evidence

Twenty game-specific functions now compile to **3,556 matching code bytes**,
with **64 generated read-only data bytes**, in nine accepted fragments. They use
ProDG 3.8.1 with `-O2 -G0`; the general inverse additionally requires
`-ffast-math`. This does not identify the original compiler release. Reconstruction
and verification used AI assistance; these functions were reconstructed from the
pinned executable rather than imported from another decompilation.

## Shared declaration and limits

[The shared header](../include/game/CMatrix.h) represents a matrix as four groups
of four floats, occupying 64 bytes. This representation is supported by several
independent observations:

- Original `_Mat_Data`, `_Mat_Unit` and `_7CMatrix.s_TempMat` symbols each have
  64 bytes of storage. `_Mat_Data` contains a four-by-four identity matrix.
- Assignment copies four 16-byte groups at offsets 0, 16, 32 and 48. It retains
  the destination in the return register, consistent with a reference return.
- `SetRight`, `SetFront`, `SetUp` and `SetPos` identify the first three components
  of those groups, in that order. They leave each fourth component untouched.
- `GetSlot(0)` writes all sixteen identity components. `Multiply` reads all
  sixteen components from each operand and computes all sixteen output values.
  The generated instructions from both routines agree with the same declaration.

`CMatrix` and its accepted method names and parameter types come from original
symbols. `CMatrixRow`, `row`, component names and parameter names are descriptive
reconstruction names. They do not claim the original nested types, member names,
access control or complete historical class declaration. Ordinary return types
and static membership are inferred from the observed calling behavior; they are
not encoded by these method symbols. The four-byte initialization flag is
represented as an `int`; its original declared signedness is not established. Constructors and other
unmatched methods are not supplied by this header; default construction must not
be assumed to reproduce the game's constructor behavior.

`CVector3` remains **opaque**. Its first three floats, at offsets 0, 4 and 8,
are supported by these setters, the original dot product and a location getter
that forwards those offsets to three float-output arguments. That does not prove
its complete size or remaining fields. `Vector3Components` exposes only this
observed prefix; the accepted functions pass vectors by reference and never
construct, copy or take the size of a complete vector object.

## Accepted fragments

| Manifest | Original text start | Functions | Code bytes | Generated data |
| --- | --- | ---: | ---: | --- |
| `matrix_core` | `0x8007a9b4` | 4 | 456 | None |
| `matrix_rotations` | `0x8007ac80` | 3 | 360 | 12 bytes at `0x8029afa8` |
| `matrix_translation` | `0x8007ade8` | 7 | 376 | None |
| `matrix_getslot` | `0x802790e0` | 1 | 92 | 8 bytes at `0x80299754` |
| `matrix_multiply` | `0x802794b8` | 1 | 836 | None |
| `matrix_8007ab7c` | `0x8007ab7c` | 1 | 260 | 12 bytes at `0x8029af9c` |
| `matrix_8007afe0` | `0x8007afe0` | 1 | 212 | 12 bytes at `0x8029afb4` |
| `matrix_8007b0b4` | `0x8007b0b4` | 1 | 176 | 8 bytes at `0x8029afc0` |
| `matrix_8007b290` | `0x8007b290` | 1 | 788 | 12 bytes at `0x8029afd0` |

The core fragment contains `InitClass`, assignment and both `BuildScale`
overloads. Translation contains `BuildTrans`, the four named setters,
`PreTranslate` and `Translate`. Rotations contains `BuildRotX`, `BuildRotY` and
`BuildRotZ`. Manifests record every individual function's original mangled name,
address, size and binding.

The three earlier fragments lie in the interval from the `matrix.cpp` compiler
marker at `0x8007a9b4` to the next file's marker at `0x8007be5c`. This is the
existing code map's inferred file-ownership evidence. The later `GetSlot` and
`Multiply` bodies have insufficient original-file evidence; their manifests
intentionally do not assign an original source file. The source files
are reconstruction fragments, not a claim about original translation-unit splits.

## Behavior, constants and original context

`InitClass` copies the 64-byte identity data into `_Mat_Unit` and writes 1 to
`s_ClassInit`. The compiler expands the copy inline. These globals remain
original context, with no source or data credit:

| Original symbol | Address | Bytes | Evidence |
| --- | --- | ---: | --- |
| `_7CMatrix.s_ClassInit` | `0x802c2100` | 4 | Global initialized object |
| `_Mat_Data` | `0x802c2104` | 64 | Private `matrix.cpp` identity data |
| `_Mat_Unit` | `0x802fbf40` | 64 | Private `matrix.cpp` BSS object |

The core manifest scopes both private dependencies to the original `matrix.cpp`
file record. The separate temporary matrix and global-constructor routines also
remain original context. Declaring private dependencies as external here is a
linking boundary between accepted source and original storage, not a claim that
the original source exported them.

`GetSlot` writes identity only for slot zero; other values leave the matrix
unchanged. Its generated constants are the floats 1 and 0. Each axis rotation
multiplies its input by the observed float **2670176.75** (`0x4a22f983`), converts
the result to an integer and calls original `MathSinCos`. The three copies of
that constant are verified at the addresses referenced by the original
instructions. The original spelling of the scale expression and the full
`MathSinCos` contract remain unproven.

`Multiply` preserves explicit output order and floating-point expression order.
It writes directly to the destination as the original does; no temporary matrix
or non-aliasing promise is introduced. Do not assume that in-place multiplication
has the same result as computing through an independent temporary. `PreTranslate`
adds a basis-weighted offset, whereas `Translate` adds components directly.
Both leave the other matrix components untouched.

## General rotation, projections and inverse

`BuildRot` uses the observed three-float axis prefix and the same angular scale
and `MathSinCos` boundary as the axis-specific rotations. It constructs the
three-by-three rotation using cached axis components, writes zero to the other
homogeneous components, and writes one at offset sixty. It does not normalize the
axis. The original source's expression spelling and parameter preconditions are
unknown; no full CVector3 construction is introduced.

`Perspective` and `Orthographic` call `GetSlot(0)` before replacing their observed
projection entries. The parameter names describe their formulas, not recovered
historical names. In particular, the retained orthographic routine writes zero
to offset sixty, as the original does; it is not replaced with a conventional
projection formula. The constants 1, -2 and 0 are compared at the addresses
referenced by the original functions.

`Inverse` separately accumulates positive and negative determinant terms. It
returns without writing the destination when the determinant is zero or its
observed relative-magnitude check falls below the single-precision constant
`1.0e-15f` (`0x26901d7d`). Otherwise it writes the inverse three-by-three entries
and translated position directly, with homogeneous zeroes and a final one.
Expression and write order are retained; in-place aliasing must not be assumed
safe. This range requires `-ffast-math` to reproduce its original comparison and
arithmetic instructions, so the high-level code is not a promise of IEEE NaN
behavior under different compiler settings.

These four routines add 1,436 game-code bytes and 44 read-only data bytes. They
lie within the original `matrix.cpp` marker interval, but their separate source
files are project build fragments. They reuse the existing matrix storage and
opaque-vector prefix view; no additional complete game type was invented.

## Verification and next work

All generated allocated sections, function boundaries, external dependencies and
constants are checked, followed by the complete reconstructed analysis image.
No instruction patches, assembly implementations, discarded generated code or
partial-match credit are used. The unchanged original-object baseline is a
separate earlier result; emulator and on-disc loading behavior remain untested.

The dependency scan originally ranked initialization, assignment and
multiplication at 173, 127 and 75 distinct unfinished callers respectively.
Those numbers overlap and are prioritization evidence, not newly reconstructed
caller code. The reusable declaration is the main benefit beyond these verified
functions: future transform-related functions can now use an independently checked
matrix representation.

The [particle fragments](ParticleRecipes.md) now use this declaration for their
two matrix-copy getters and an `Orthonormalize` wrapper. Only the wrapper is
reconstructed; `CMatrix::Orthonormalize` is declared as an original external
dependency and receives no new matrix-source credit.

The [camera fragments](Camera.md) reuse the same representation and declare the
original shared `CMatrix::s_TempMat` (64 bytes at `0x802fbf00`) for pre- and
post-transform operations. The header also exposes `Rotate(const CVector3 &,
float)` as an external original method. Neither that method nor the temporary's
storage earns new matrix-source credit. The camera wrappers keep vectors opaque.

Useful next work is to recover enough `CVector3` layout/constructor evidence to
support value parameters and local vectors, then extend `Rotate`, `FastInverse`, `Orthonormalize` and the remaining matrix
functions. A research `TibToMOHFL`
implementation has the expected 88-byte size but still differs in instruction
scheduling/register allocation; it earns no credit. Do not extend the opaque
vector declaration simply to make an isolated candidate compile.
