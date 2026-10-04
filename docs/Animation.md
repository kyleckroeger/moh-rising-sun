# EAGL animation reconstruction

The animation work adapts the public NFS Most Wanted reconstruction to Rising
Sun's older `EAGLAnim` interface. It includes scalar/vector delta decoding,
compressed channels, composition, events, skeleton poses, run blending, raw and
cyclic channels, allocation support and stateless control interfaces. The first
two delta-compressed animation decoder units cover
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
against the original tables. The default base implementations and memory-map methods are now reconstructed;
allocation callbacks remain external original context.

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
by these fragments. Exact historical header organization and visibility remain
unknown. Undecompiled animation formats earn no credit.
Complete-image verification is required before snapshot publication; no animation
runtime or emulator testing has been performed.


## Composition, masks and base methods

`CompoundChannel` adds 19 functions and 4,604 code bytes, with its complete
80-byte constant pool checked separately. Its map prefix has attributes at four,
unsigned halfword channel/frame counts at eight/ten, then a variable pointer
array. The function object is 28 bytes: inherited map at twelve, channel array
at sixteen, four-byte FPS-enable flag at twenty and byte FPS at twenty-four.
Its deleting destructor and all evaluators independently support this storage.

The composition routines initialize and dispatch subchannels through the original
memory-manager slots. They preserve the original new-map-before-cleanup order,
reverse iteration and non-short-circuit accumulation of child results. The
attribute helper retains a two-byte ID, its by-value copies and the returned
pointer slot's high byte for FPS. Reference member names remain descriptive.
The composition vtable stays original context under a scoped native interface
pragma; no generated data is silently removed.

The six `BoneMask` functions add 524 bytes. Construction, complement, equality
and bit mutation agree on eight 32-bit words. Separate constructor and operation
fragments preserve the compiler's inline constructor copies without duplicate
credit. Seventeen default `FnAnim` methods add 128 bytes; their no-op, false and
null results are present in the original, not placeholders for unfinished code.
Six `FnAnimMemoryMap` methods add 120 bytes.

The recovered base destructor corrects the initial allocator placement:
`FnAnimMemoryMap` performs ordinary deletion, while the decoded leaf classes
use the EAGL free callback. The class-body allocation include now belongs to those
leaf classes. Both previously accepted delta decoders remain byte-identical after
this correction. The original macro/header organization is still unknown.

## Compressed scalar and quaternion channels

`DeltaCompressedData` contributes four functions and 3,592 code bytes; `DeltaChan`
contributes 25 functions and 9,136 bytes. Their complete 32/128-byte constant pools
also match. The former has a four-byte count/quantization prefix followed by
12-byte minimum/range/initial-value records and the delta stream. The latter's
plain and keyed maps have variable index arrays starting at ten and fourteen;
rounded C++ object size is not used as serialized-header length.

Constructors, buffer accesses and deleting destructors support the two 36-byte
function-object layouts. Scalar and quaternion evaluators retain their cache,
mask and interpolation behavior. The target reallocates when the new DOF count
is greater than or equal to the previous count; this is not changed to a
conventional capacity check. The indexed four-bit decoder retains its unusual
initial nibble selection. Masked decompression supports the two observed sample
widths only.

Key-time index minus one denotes the implicit initial sample at time zero.
The equivalent expression `lowKey - 1 == -1` preserves the original compiler's
separate comparison in the four keyed evaluators. The source retains this index
interpretation rather than substituting machine instructions. Masked evaluation
uses an eight-bit bone index; bone enumeration retains its observed 16-bit result.
Original vtables remain external context for these channel fragments.

## Event and allocation support

The event, bank, allocator, scratch-buffer and raw-pose support fragments add
35 functions and 4,028 code bytes. Each complete generated constant pool is
checked. Event traversal retains looping and cached-time behavior. CSIS records
have an eight-byte event, a twelve-byte dictionary prefix with variable metadata,
and a sixteen-byte parameter view. The original function reserves a 255-integer
parameter buffer. Raw event records occupy sixteen bytes after their eight-byte
channel prefix. Original handler accesses place the successor at zero and native
virtual pointer at four; no handler storage beyond the observed interface is
inferred. Both function wrappers have 24-byte extents established by deletion.

`AnimBank` describes only its observed twenty-byte prefix. Factories use the
original embedded object offsets: minus 24 for stateless formats and minus 16
for pose animation. They call through the established base interface. Compound
construction initializes the independently established 28-byte object. Type-ID
copies and destruction retain the observed halfword temporary.

Memory-pool fragments reconstruct cleanup, indexed free, usage and map-based
factory behavior. Embedded animation types bypass deletion; ordinary objects
retain the original virtual destruction followed by type-based free. The named
free-list storage has 26 pointers but its size table has only 25 halfwords; these
extents are deliberately not conflated. The manager deleting destructor passes
four bytes. Original pool storage and allocation bodies still outside the
accepted fragments remain external.

Scratch buffers have twelve-byte records and a named three-record global array.
Freeing decrements the reference count first and clears only the buffer pointer.
Raw-pose initialization preserves its two variable signature tables, native helper
addresses and invalid-type diagnostic. Its map prefix is sixteen bytes; the
function wrapper is twenty bytes, independently established by deletion. The
interpolated raw-pose evaluator remains original context; the completed frame
evaluator and six transformation helpers are described below.

## Stateless control interfaces

The stateless F3 and quaternion control fragments contribute ten functions each,
852 and 896 code bytes respectively. Each complete native 184-byte vtable and
8-byte conversion constant pool also matches. The larger evaluators remain
original context; a matching control fragment does not imply decoded format
coverage.

Both function objects occupy 24 bytes, with previous-key halfword at sixteen,
FPS-enable/FPS bytes at eighteen/nineteen and mask pointer at twenty. The Q
constructor initializes the previous key; F3 does not. F3's serialized prefix
holds key count at sixteen and byte bone counts at eighteen/nineteen. Q has a
companion F3 function pointer at sixteen, then key/bone counts at twenty through
twenty-three. Q's bone enumerator forwards to that companion when present.
F3 frame-count conversion retains unsigned-halfword truncation; Q uses a signed
integer result. These differences are established by the original instructions.
All formats remain runtime untested.

## Skeleton poses and matrix callback

The skeleton fragment adds six functions and 3,916 code bytes, with its complete
four-byte constant pool. An adjacent private matrix callback adds 708 code bytes
and the original four-byte initialized `MatrixMultiply` pointer. The latter's
local function binding is scoped to the original `System.cpp` file record; that
scope is not used to infer ownership of unrelated global functions.

The serialized skeleton prefix occupies sixteen bytes, followed by 112-byte
bone records. Independent pose, skinning and mirroring loops establish the record
stride and its scale, signed parent index, quaternion, translation, signed mirror
index and 64-byte inverse base matrix. The prefix's first eight bytes remain
unknown storage; only its bone count and inverse-scale pointer are interpreted.
The implementation reuses the independently checked transform and coordinate
types. Reference headers supply attributed field and interface names; their empty
Skeleton implementation supplies no method bodies.

Mirroring preserves separate masked, unmasked, in-place and separate-buffer
paths. Paired bones exchange quaternion and translation components with the
original sign changes. Separate-buffer paths also copy scale xyz; neither path
silently initializes untouched pose slots. Nonlocal mirroring adjusts the root
quaternion and translation. Mask checks apply to the source bone.

Pose conversion retains the extra first-column scale, parent composition and
root assignment. Unselected transforms remain untouched. Still-pose generation
uses inverse bone scales when available and writes the original homogeneous
components. Skinning composes the current transform with the inverse base matrix,
then transposes the result.

The callback's argument order is output, first input, second input. Its arithmetic
uses a real 64-byte temporary before copying to the output, preserving aliasing.
It adapts the project's existing matrix expression sequence with the target's
operand order; it must not be substituted for the rendering helper's different
calling interface. No new imported implementation, compiler flags, generated-code
exclusions or runtime claims are introduced.

## Single-axis quaternion delta format

`FnDeltaSingleQ` reconstructs eight functions and 13,472 code bytes, plus its
complete 160-byte constant pool and native 192-byte virtual table. Native unused
function stripping removes the separately emitted inline `InitBuffersAsRequired`
body; its constants remain verified, and the discarded code receives no credit.
The compiler profile otherwise matches the existing animation units.

The function object occupies 48 bytes, as established by construction, deletion
and field accesses. Its sixteen-byte serialized map is followed by fourteen-byte
per-bone range records, then bins. Physical samples contain two bytes: the selected
axis component and quaternion w. Each one-byte delta holds two four-bit values.
Expanded ranges occupy 28 bytes, including the observed byte-sized axis index.
The record/header extents follow target strides rather than C++ padding guesses.

Initialization allocates three independent quaternion buffers. The destructor
frees all three only when the previous-sample allocation is nonnull. The reference
`GetArrays` interface returns range and bin pointers by reference; this reproduces
the observed access ordering without artificial memory barriers. Encoded angles
expand over minus pi through pi, and the two constant Euler components establish
pre- and post-alignment quaternions for the selected axis.

Backward requests reload the physical sample; forward requests accumulate
component deltas. Linear interpolation normalizes the result before specialized
quaternion products restore the constant alignment. Output uses the established
twelve-float pose stride and quaternion offset four. Masked evaluation preserves
its previous-key cache and selects physical, delta and output updates separately.
The generic `Eval` wrapper retains the original virtual call and null mask argument;
no unverified assumption of null-safe runtime behavior is added.

The reference implementation bodies are empty. Mathematical bodies are newly
reconstructed from the pinned instructions using the already checked scalar/vector
decoder structure. Helper names describe their operations and do not claim original
inline boundaries. Every generated allocated byte is checked; runtime behavior in
the game remains untested.

## Raw-pose, cyclic and linear channels

Six raw-pose transformation helpers and the frame evaluator add 2,380 code bytes.
Each complete generated constant pool matches. Quaternion/translation copies
advance the input pointer per component. Interpolation retains the reference's
quaternion sign selection and normalization, and the target's translation math.
Euler conversion follows the original half-angle trigonometric calls and degree
conversion. The original 28-byte `qt0` global remains external storage.

The frame evaluator consumes a count and patched helper addresses from each
signature record. Masked-out channels advance their input by three or four floats
according to the helper, without writing output. The larger interpolated evaluator
has a code-exact research candidate but an unresolved complete constant pool; it
is excluded from accepted source and progress. No padding is removed to accept it.

The cyclic wrapper adds six functions and 1,460 bytes. Four evaluators agree on
its 28-byte extent and start/end/length/child fields, forwarding through the
established virtual interface. Time wrapping retains signed truncation and the
original unusual below-start expression. Four independent conversion pools are
checked; its constructor and virtual table remain original context.

The raw-linear wrapper adds four functions and 708 bytes. Its deleting destructor
establishes twenty bytes, including the four-byte interpolation flag. The map has
an eight-byte prefix with halfword DOF/frame counts, an index array aligned to an
even halfword count, then float frame data. Clamping, interpolation and per-element
count reloads are preserved. The two complete conversion pools also match.
Reference method bodies for these wrappers are empty; their implementations are
reconstructed from original instructions using attributed interface names.

## Run and pose blending fragments

Seven run-blender functions add 3,940 code bytes and verify 72 constant bytes.
They cover destruction, pose evaluation, weight changes, velocity alignment and
blending, root-quaternion computation and root alignment. The 128-byte object
extent follows its deleting destructor; repeated accesses establish its arrays,
four child evaluators, timing values and alignment quaternion. Unproven storage
at `0x6c` remains unknown, and the owned allocation at `0x7c` is an untyped pointer.
The associated phase-channel declaration describes only its observed twelve-byte
prefix. Remaining timing, facing and initialization methods stay original context.

Weight changes preserve adjacent-channel reuse and optional velocity channels.
Pose evaluation retains the original use of unmasked child calls, scratch storage,
and cycle/root alignment even though its public signature accepts a mask.
Quaternion multiplication uses ordinary float-array inputs, preserving the target's
component reloads where arrays may overlap. There are no register constraints or
special compiler flags.

The static pose-blending function adds 1,048 bytes and its complete eight-byte
constant pool. It blends and normalizes quaternions, linearly blends translation,
and preserves scale and untouched pose components. Masked paths skip unselected
bones. The shared header exposes this static interface only; the pose-blender
instance layout and larger transition evaluators remain unreconstructed.

The quaternion blending helper retains CC0 `AnimUtil.h` attribution. The original
run/pose implementation bodies are not supplied by that reference; target
instructions establish the reconstructed control flow and pointer behavior.
All these fragments require complete-image verification and remain runtime untested.
