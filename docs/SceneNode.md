# Scene-node virtual tables

This note records the virtual-slot order of `ISceneNode`, `IMovingSceneNode` and
`IParticleSystem`, recovered from original virtual tables in the pinned GR8E69
executable. Claude Code assisted the analysis and verification.

## Evidence

The executable has no vtable symbols, but constructors store table addresses and
each entry's function pointer resolves to a named original function. Entries are
eight bytes: a signed 16-bit `this` adjustment followed by the function pointer.
Calls load the adjustment with `lha` and the target with `lwz` at the same slot.
Slot 0 is zero in every table examined.

| Table | Address | Slots | Evidence |
| --- | --- | ---: | --- |
| `IObserver` | `0x802e9aa0` | 1-4 | Already documented in [Observers.md](Observers.md) |
| `ISceneNode` | `0x802e1680` | 1-67 | Slot 2 is `_._10ISceneNode`; the entry after slot 67 is zero |
| `IParticleSystem` | `0x802e2048` | 1-107 | Stored by `__15IParticleSystem`; slot 2 is `_._15IParticleSystem` |
| `CParticleSystem` | `0x802e18c8` | 1-108 | Stored by `__15CParticleSystem`; the entry after slot 108 is zero |

Slots 1-4 match the existing `IDestructible`, `ISubject` and `IObserver`
declarations. `ISceneNode` introduces slots 5-67. `IMovingSceneNode` has no
out-of-line destructor or emitted table, but `IParticleSystem` and at least twelve
other derived tables, including `CStaticObject`, `CHierObject`, `CWeapon` and
`CBullet`, place `Reset__16IMovingSceneNode` at slot 68. Slots 68-88 resolve to
`IMovingSceneNode` symbols or their overrides, and `IParticleSystem` begins at slot
89. `CParticleSystem` adds only `UseFade` at slot 108.

The inlined base constructors in `__15IParticleSystem` write the observer fields
at `+4` through `+20`, including the observer's owner pointer to itself at `+16`.
This agrees with the 24-byte `IObserver` prefix. They also zero words at `+24`
through `+52`; which base class owns those words is not established, so no
scene-node data members are declared.

## Declarations

`include/game/SceneNode.h` declares `ISceneNode` and `IMovingSceneNode`;
`include/game/ParticleRecipe.h` declares `IParticleSystem`. Both use
`#pragma interface`, like the observer headers, so no accepted object emits a
table. The headers were generated from the table entries. Method names, parameter
types and constness come from the mangled symbols; enum parameter types
`EClsnId` and `ISceneNode::EVolumeType` have placeholder members only.

Return types are not encoded in these symbols. Each is a `void` placeholder
unless an accepted override's body establishes an ABI result (`IsDrawEnabled`,
the `IParticleSystem` flag getters and `GetDef`) or a cast returns `this`
(`AsMovingNode`, `AsProceduralParticleSystem`, `AsMeshParticleSystem`). Callers
that use a placeholder result need evidence before its declaration is changed.

`CScene` is a member-only view declaring `Remove(ISceneNode &)` for the external
`g_scene` object.

## Verification

ProDG 3.8.1 compiles virtual calls through the declared order at the original
offsets: `Stop` calls slot 91 (`+0x2d8`), `SetLocalToWorld` slot 71 (`+0x238`),
`Terminate` slot 1 and `Destroy`'s deleting destructor slot 2. Any missing or
misplaced declaration before those slots would shift those offsets. All particle
fragments accepted before the hierarchy was introduced still produce identical
objects, and the complete rebuilt analysis image is identical.

## Next work

The default `ISceneNode` and `IMovingSceneNode` bodies near `0x80279130`-`0x80279a00`
include about 95 four- and eight-byte functions. They need return types where they
return values. The same slot map applies to every class derived from
`IMovingSceneNode`; each derived table should be checked against it rather than
assumed.
