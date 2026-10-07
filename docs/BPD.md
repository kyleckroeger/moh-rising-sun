# BPD conversion and lighting-volume evidence

[kyleckroeger's PS2 gameplay-layout research](research/ps2/bpd-format.md) identified
the header, lighting records and relevant GameCube conversion routines. This batch
uses those leads to reconstruct **13 functions and 1,380 executable bytes** from
the pinned GR8E69 executable, with Codex assistance. PS2 data was not substituted
for GameCube evidence, and no original files or dumps are included.

## Accepted fragments

| Fragment | Scope | Functions | Code bytes |
| --- | --- | ---: | ---: |
| `bpd_endian` | Header, property records, animated lights, light volumes and counted lists | 9 | 1,168 |
| `endian_scalar` | Float and signed-int conversion wrappers | 2 | 64 |
| `endian_short` | Signed-short conversion wrapper | 1 | 32 |
| `bpd_light_volume` | `IsPointInLightVolume` | 1 | 116 |

The conversion block lies between the `propdat.cpp` and `proxtrig.cpp` compiler
markers. The point test lies between the `proxtrig.cpp` and `flexprop.cpp` markers;
the scalar wrappers lie in the `endian.cpp` marker interval. These are inferred
original file groups, not recovered translation-unit boundaries. The manifests
retain the exact function symbols, bindings, sizes and external dependencies.

## What the conversions establish

The `ChangeEndian` overloads and template names encode their argument types. Together
with each call's field address, they establish the accessed integer, halfword, float
and pointer types in `include/game/BPD.h`. Member names are reconstruction choices.
Descriptive names such as header table counts, bounding minima/maxima and animated
light colors/times follow the contributed research; conversion calls alone do not
prove those meanings. Fields without sufficient semantic evidence retain offset names.

The header view reaches `+0x6c`. Pointer types match the research's twelve table
locations, including the string pointer at `+0x54` after its integer at `+0x50`.
The word at `+0x0c` is untouched. The routine calls `ChangeEndian(int&)` on `+0x24`
**three times**; all three calls are preserved in their original order.

`xyzProperty_Struct` converts four leading fields followed by seven floats.
`MOH_core_Struct` converts selected halfwords and pointers beginning at `+0x2c`,
leaving its preceding region untouched. The mechanic, enemy, environment-modifier
and animated-light routines call the core conversion at offset zero before their
own field conversions. The `core` member models that shared prefix without claiming
historical C++ inheritance. Unknown gaps and untouched fields remain opaque.

For animated lights, the calls establish a signed halfword at `+0x72`, an
`unsigned long*` at `+0x74` and a `float*` at `+0x78`. This supports the color/time
array interpretation in the PS2 notes, but does not prove timing units, mode values,
the full record size or the encoding of packed colors on GameCube.

The light-volume conversion touches six floats, three integers and pointers to
`PropPlane4` and `BPDLight`. Its view ends at the integer at `+0x28`; it deliberately
does not claim the complete 48-byte allocation described by the PS2 research.
None of these prefix views may be used to allocate complete game objects or infer
unverified array strides. The pointed-to path-finding and light types stay opaque.

`EndianSwapList` converts the leading unsigned-long count, then entries 1 through
that count inclusive. It re-reads the converted count at each loop test and retains
the original unsigned comparison. It adds no malformed-input checks.

The accepted float/int/short wrappers delegate through unsigned references to
convert the underlying representation, without numeric float conversion. The
unsigned scalar routines and all `ChangeEndian<T>` implementations remain external
original code. This is a target-specific 32-bit interface, not a portable file parser.

## Independent lighting-volume check

`IsPointInLightVolume` independently reads the integer plane count at volume
`+0x18` and plane pointer at `+0x1c`. It steps through **16-byte** plane records,
using floats at offsets 0, 4, 8 and 12 to compute:

```text
plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.d
```

Every result must compare greater than or equal to zero. Negative or unordered
results return zero immediately; boundary points pass. A nonpositive plane count
returns one without examining any planes. The reconstruction preserves the original
floating-point operation order and generated fused operations.

The point remains an opaque `CVector3`, accessed through the existing
[`Vector3Components` prefix interface](Matrix.md). No complete vector size,
constructor or normalization requirement is inferred. The plane stride and four
component accesses support the shared `PropPlane4` record view.

## Verification and next work

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces the
individual functions and every allocated section in these fragments. The point
test's four-byte zero constant at `0x802a6c58` is checked separately and earns no
code credit. No instructions are patched, no assembly bodies are substituted and
no generated functions or section bytes are discarded or clipped.

The complete rebuilt 2,860,576-byte analysis image also matches the original,
including its header, allocated ELF bytes, entry point and BSS extent. Follow the
[build, test and snapshot process](Progress.md) for future changes. Runtime,
emulator behavior, GameCube `.bpd` serialization, pointer relocation, and the full
level loader remain unverified by this batch.
The shared conversion interfaces and scoped layouts provide starting points for
that work. [LightVolumes.md](LightVolumes.md) covers light-volume priority
selection and transitions, and records the blending read from `GetVolume`. Light
field layouts and animated-light playback still require independent GameCube
investigation.
