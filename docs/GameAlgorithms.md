# Game algorithms and observer evidence

This extension adds **56 STLport functions (21,284 code bytes)** and **nine
weak-pointer event handlers (648 game-code bytes)**. The library algorithms use
the pinned, attributed STLport 4.5.3 source; the game declarations and handlers
were reconstructed with AI assistance. Claude Code supplied an independent,
read-only analysis of four original function excerpts. Its hypotheses were
checked against additional symbols, callers, constant data and compilation.
That review did not itself build the game or establish all declarations below.

The explicit instantiation files are project build fragments, not recovered
historical source-file boundaries. Every complete generated function, symbol,
external reference and allocated section must pass the existing strict checks,
followed by the complete analysis-image comparison. Nothing is patched or masked.

## Scope and unknowns

The three new headers in `include/game-containers/` describe only the operations
retained here. Original mangled symbols supply class/template/function names.
Instructions, callers and original vtables supply storage bounds, comparisons,
copy side effects and virtual dispatch. `field_XX`, storage helpers, member names,
access control and inline factoring are reconstruction choices. Their names do
not claim recovered field meanings or original source spelling.

Unknown spans can contain both fields and padding. The records are not complete
engine APIs, and enclosing classes used for nested names have no established
size. Do not construct incomplete pointee types or combine these scoped shells
with independently defined versions of the same classes. Equivalent C++ source
can produce the same bytes; the match verifies the retained operation, not a
unique historical declaration.

## Sorting and rendering

| Family | Functions | Code bytes | Evidence |
| --- | ---: | ---: | --- |
| `ScoreInfo` sorting | 13 | 2,748 | 20-byte stride and full value copies; signed descending comparison at offset `0x0c`. |
| `HandlerLeaderboard::Player` sorting | 12 | 5,272 | 32-byte stride; stateful comparator uses signed fields at `0x0c..0x1c`. |
| `DrawCommandData` heaps | 3 | 1,864 | 96-byte stride; CMatrix assignment plus a complete 32-byte aggregate copy. |
| `TargetInfo` sorting | 13 | 6,020 | 48-byte stride; observer subscription lifecycle, byte/float copies and a 16-byte vector-like value. |
| Resource containers | 11 | 4,904 | Reference counts, original virtual deletion, partition ordering and observed record storage. |
| Pointer lookup/list/deque helpers | 4 | 476 | Pointees remain forward declarations; no game object is constructed or sized. |
| **Library total** | **56** | **21,284** | |

`ScoreInfo::__unguarded_linear_insert` at `0x8028c0e0` and partitioning at
`0x8028be94` independently establish the record stride, copies and signed ordering.
`CHeadsUpDisplay::DrawFragCount` at `0x8017399c` supplies the original sort callers.
The other sixteen record bytes remain unnamed storage.

The leaderboard comparator first checks its signed four-byte state. If that is
nonnegative and the two `0x18` fields differ, it places the record whose `0x18`
field equals that state first. It then compares `0x10` descending, `0x0c`
ascending, `0x1c` ascending, and finally `0x14` ascending, all signed. The original
`SortByTeamAndScore` name suggests meanings but does not establish every field's
name. `0x8028f284`, `0x8028ec14` and the heap helpers agree on these rules.
`HandlerLeaderboard::HandleEvent` at `0x80198120` is the original caller. The
existing reserve fragment and new algorithms share one record declaration.

Rendering comparisons order the unsigned word at `0x40`, then the unsigned word
at `0x44`. This is also the numerical order of a big-endian unsigned 64-bit key;
the source uses two observed word components without asserting the original
64-bit type or field spelling. `0x802885e4` calls the original, already rebuilt
`CMatrix::operator=` for the first 64 bytes and copies the remaining 32 bytes as
one aggregate. `CDrawContext::AddCommand` at `0x8012ea14`, whose signature names
`DrawCommand`, independently copies the same aggregate and calls `push_heap`.
The custom `DrawCommandData` copy constructor models these operations directly;
whether the original delegated through an inline CMatrix copy constructor is
unconfirmed. No additional CMatrix constructor behavior is claimed.

## Target selection and observer lifecycle

The original table at `0x802e48b0` is named
`_vt.t7WeakPtr2Z10ISceneNodei8`; the table at `0x802e9aa0` is `_vt.9IObserver`.
This resolves the first 24 bytes of `TargetInfo` to an observer-backed weak
pointer, rather than a trivially copied pointer or a newly invented TargetInfo
vtable. The node at offset eight contains two links followed by its owner pointer.
A subject pointer occupies offset `0x14`. Original destructors establish the
`IObserver` → `ISubject` → `IDestructible` base chain; named vtable entries establish
the declared virtual-method order. The pointer stored at subject offset four is
the observer-list head, independently read by `ISubject::AddObserver` at
`0x8014ae44`.

Copies initialize an empty observer, unlink the destination node, assign its
subject, then call the original `ISubject::AddObserver` when nonnull. Assignment
performs the same unlink/subscription operation while retaining destination
identity. The `IObserver` destructor is an external call, now reconstructed in the
[observer extension](Observers.md). Thus
sorting preserves the observed nontrivial lifetime operations. The two-part
`ObserverLinkBase`/`ObserverLink` and `ObserverHead` are project storage models;
their original class names and exact implementation hierarchy are unknown.

The remaining TargetInfo fields are a copied byte at `0x18`, a float sort key at
`0x1c`, and a 16-byte payload at `0x20`. The payload copies three floats and sets
the fourth to **1.0f** on construction. Assignment writes three floats and returns
a value, producing the original temporary with that fourth component. The helper
is deliberately named `TargetVectorStorage`: this evidence does not by itself
establish that its original type was CVector3, so the shared CVector3 declaration
remains opaque. `DetectTargets` at `0x800b9404` is the original sort caller.

Nine separate four-byte constant pools, `0x8029f524` through `0x8029f544`, contain
1.0f and are checked at the addresses referenced by their respective generated
functions. They add 36 verified data bytes and no code-progress credit.

These target-sorting fragments require `-ffast-math` in addition to the existing
ProDG 3.8.1 `-O2 -G0 -fno-exceptions -fno-implicit-templates` profile. The other
new container/algorithm families use the existing profile. This is evidence for
compatible compilation of these ranges, not proof of the historical compiler
release or flags for the entire game. Do not infer IEEE NaN ordering guarantees
from the C++ comparison: the original and compiled instructions, including this
floating-point profile's conditional behavior, are the checked result.

The nine retained `WeakPtr<T, N>::HandleEvent` instances test the original event
mask (`8`, or `12` for one ISceneNode instance), then unlink and clear the subject
when any selected bit is set. Each is 72 bytes and is **reconstructed game code**.
The pointee types are only forward declarations. `ESubjectEvent` is retained as
an enum for ABI/mangling; its placeholder zero enumerator does not claim the
original event enumeration. The mask values come from original instructions and
mangled non-type template parameters. The [observer extension](Observers.md)
adds subject/observer methods and the deferred-destruction queue.

## Resource containers

The original Resource destructor at `0x8027c918` writes the named Resource vtable
at offset four. Container copies increment the count at offset zero; release
checks the decremented count, then invokes the original virtual destructor when
it reaches zero. The retained profile models that eight-byte prefix, never a
complete EAGLLoader or State object. `Resource::Pointer<T>` uses an explicit
prefix view while leaving those pointees incomplete; the cast asserts only the
observed zero-offset prefix for these operations, not their full inheritance.
`Release` and `field_00` are descriptive source names, not recovered method/member
names. No virtual destructor implementation or original vtable earns credit.

| Container | Functions | Code bytes | Observed value/comparison |
| --- | ---: | ---: | --- |
| `vector<Resource::Pointer<EAGLLoader> >` | 1 | 368 | Four-byte managed pointer; null-aware copy/release. |
| Skeleton partition → Choreographer context | 4 | 2,284 | 36-byte node: pointer key and 16-byte context. |
| String → `Resource::Pointer<State>` | 4 | 1,452 | 36-byte node; original `strcasecmp` on string data. |
| MatrixBlend/LinearBlend subtask vectors | 2 | 800 | 12-byte stride: two floats followed by a managed pointer. |

Partition comparison treats a null pointer's signed order value as zero, otherwise
reads offset `0x24`; it orders those values first and pointer addresses second.
The declaration exposes only that prefix and is never used to allocate a complete
Partition. `Choreographer::SetState` at `0x8008055c` independently uses this order
and constructs/updates the 16-byte context: a managed pointer, two copied words
and a float. The two words have no established signedness or complete semantic
types. `Pointer<Resource>` represents the observed managed-prefix operations in
Context/SubTaskInfo without guessing the original member specialization.

## Verification boundary

The scoped models above are used only by the enumerated fragments. All allocated
output, complete function boundaries and original symbol bindings are checked.
New upstream algorithm/heap dependencies are copied unmodified and hash-pinned
with their notices in `stlport-source.json`. The same complete source build,
validation suite and snapshot capture gates in [Progress.md](Progress.md) apply.
The original-object baseline is separate; runtime remains untested.
