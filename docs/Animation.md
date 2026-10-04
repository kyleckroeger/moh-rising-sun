# EAGL animation reconstruction

The animation work adapts the public NFS Most Wanted reconstruction to Rising
Sun's older `EAGLAnim` interface. Two delta-compressed animation decoders cover
22 functions and 15,012 executable bytes: `FnDeltaF1` contributes 6,388 bytes and
`FnDeltaF3` contributes 8,624. Each generated 184-byte virtual table and 80-byte
constant pool is checked separately and earns no code credit. AI assistance from Codex and Claude Code was
used for reconstruction and review.

## Reference and target evidence

The reference is [dbalatoni13/nfsmw at revision
1f2cdd7996791c81a580b3f7b36b44d4f9f6719c](https://github.com/dbalatoni13/nfsmw/tree/1f2cdd7996791c81a580b3f7b36b44d4f9f6719c/src/Speed/Indep/Src/EAGL4Anim).
Its CC0 license and attribution are preserved in
[`src/eagl_anim/NOTICE`](../src/eagl_anim/NOTICE) and
[`LICENSE.nfsmw`](../src/eagl_anim/LICENSE.nfsmw). This is reconstructed game code,
not restored SDK/library credit. Reference names are useful reconstruction names;
they do not establish original Rising Sun member spelling.

Original mangled symbols establish the classes and method interfaces. The
`FnDeltaF1` constructor, destructor and evaluators independently establish a
48-byte object: previous key/block/value fields at `0x10`/`0x14`/`0x18`, next
key/block/value fields at `0x1c`/`0x20`/`0x24`, a mask pointer at `0x28`, and a
pointer to 16-byte unquantization records at `0x2c`. The deleting destructor
passes 48 to the original allocation callback. The base constructor zeroes its
unknown word at offset four and animation pointer at offset twelve. The unknown
word's original type remains unspecified.

The memory-mapped animation header occupies twenty bytes before its per-bone
records in both formats. The original accesses establish the index/time pointers, frame and bone
counts, bin-length exponent and constant-bone count. Each following `DeltaF1` record has
two floats and two unsigned halfwords, with a twelve-byte stride. Physical
samples are unsigned halfwords; delta samples are bytes. Arithmetic and integer
conversion order follow the target, including signed promotion during sample
unquantization and unsigned conversion in direct frame-count casts.

The original `BoneMask` copy constructor at `0x801eddc8` copies 32 bytes, supporting
the eight-word mask view independently of the reference. Masked evaluation uses
an eighty-byte temporary index array, established by its stack accesses. No new
bounds checks are added. Base and memory-manager virtual-slot ordering is checked
against the original tables. Most base implementations and allocation callbacks
remain external original context.

## Three-component decoder

`FnDeltaF3` uses the same 48-byte object layout with animation type 20; the
`FnDeltaF1` constructor stores type 21. Its value buffers contain 16-byte `COORD4`
elements, with three components used, and its decoded range records occupy
48 bytes. Each encoded per-bone record has a 36-byte stride: three physical
minimum floats at offset zero, three range floats at twelve, and unsigned
halfword minimum/range triplets at twenty-four and thirty. Original allocation
sizes, accesses and the three-iteration decoding loop support these layouts.
The coordinate declarations reuse the established [transform interface](Rendering.md).
Reference field names remain descriptive; there is no claim to original member names.

Both evaluators recover physical samples at bin boundaries, accumulate forward
or reverse deltas, cache the adjacent sample and interpolate when required.
The masked evaluator preserves the original mask-cache invalidation and constant
bone handling. The target's expression order is preserved, including differences
between its masked and unmasked interpolation formulas. In `EvalSQT`, one shared
frame-loop counter and the original pointer/index update order are needed to
reproduce the compiler's scheduling. These source decisions follow the target
instructions; no register pins or instruction substitutions are used.

## Compilation and verification

The working profile is ProDG 3.8.1 with `-O2 -G0 -fno-exceptions
-fno-implicit-templates`. It is not proof of the original compiler release.
The compiler emits the virtual tables in their native named
`.gnu.linkonce.d._vt.Q28EAGLAnim9FnDeltaF1` and
`.gnu.linkonce.d._vt.Q28EAGLAnim9FnDeltaF3` input sections. The linker resolves each
weak object to the original global binding. The manifest places that complete
section at the original `.data` address; there are no instruction or object
patches.

The verifier permits only named native vtable sections for this additional data
mapping. It checks the complete original object, compiler ownership and binding,
linked ownership and original binding, allocated-section flags, relocations,
external definitions and all bytes. Mutation tests reject partial/moved vtables,
altered metadata and attempts to map arbitrary link-once sections or code.
Existing code and BSS mapping restrictions remain in force.

The shared class declarations describe the checked interface and storage needed
by these fragments. Exact historical header organization, visibility and the
location of the inherited inline allocation operator are not established.
The original base bodies and undecompiled animation formats earn no credit.
Complete-image verification is required before snapshot publication; no animation
runtime or emulator testing has been performed.
