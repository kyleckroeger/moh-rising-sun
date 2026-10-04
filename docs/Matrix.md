# CMatrix reconstruction evidence

Sixteen game-specific functions now compile to **2,120 matching code bytes**,
with **20 generated read-only data bytes**, in five accepted fragments. They use
ProDG 3.8.1 with `-O2 -G0`, the same working profile as MathFun and the allocation
operators. This does not identify the original compiler release. Reconstruction
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

The core fragment contains `InitClass`, assignment and both `BuildScale`
overloads. Translation contains `BuildTrans`, the four named setters,
`PreTranslate` and `Translate`. Rotations contains `BuildRotX`, `BuildRotY` and
`BuildRotZ`. Manifests record every individual function's original mangled name,
address, size and binding.

The three earlier fragments lie in the interval from the `matrix.cpp` compiler
marker at `0x8007a9b4` to the next file's marker at `0x8007be5c`. This is the
existing code map's inferred file-ownership evidence. The later `GetSlot` and
`Multiply` bodies have insufficient original-file evidence; their manifests
intentionally do not assign an original source file. The five new source files
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

## Verification and next work

All generated allocated sections, function boundaries, external dependencies and
constants are checked, followed by the complete reconstructed analysis image.
No instruction patches, assembly implementations, discarded generated code or
partial-match credit are used. The unchanged original-object baseline is a
separate earlier result; emulator and on-disc loading behavior remain untested.

The dependency scan originally ranked initialization, assignment and
multiplication at 173, 127 and 75 distinct unfinished callers respectively.
Those numbers overlap and are prioritization evidence, not newly reconstructed
caller code. The reusable declaration is the main benefit beyond these 2,120
bytes: future transform-related functions can now use an independently checked
matrix representation.

Useful next work is to recover enough `CVector3` layout/constructor evidence to
support value parameters and local vectors, then extend `BuildRot`, `Rotate`,
`FastInverse` and the remaining matrix functions. A research `TibToMOHFL`
implementation has the expected 88-byte size but still differs in instruction
scheduling/register allocation; it earns no credit. Do not extend the opaque
vector declaration simply to make an isolated candidate compile.
