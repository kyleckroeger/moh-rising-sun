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

Return types are not encoded in these symbols. They are chosen as follows:

| Return type | Evidence |
| --- | --- |
| `As*` casts return the named class (`CStaticObject *`, `CBullet *`, ...) | Each named class has its own override that returns `this` unchanged; `AsAnimatedPlayerObject`'s override belongs to `CAnimatedPlayer` |
| `GetAIObject` returns `CAIObject *`; `GetAIDoodad` returns `CAIDoodad *` | `CPlayerObject`'s overrides return a pointer field and the address of an embedded member; both classes have original symbols |
| `GetAttachedLight` returns `CLight *` | `SetAttachedLight` takes `CLight *`; `CStaticObject`'s override returns a pointer field |
| `GetLightVolume` returns `BPDLightVolume *` | `Enter`/`ExitLightVolume` take that type; `CAnimObject`'s override returns `CLightVolumeManager::GetVolume()` |
| `GetName` returns `const char *` | `CBotObject`'s override returns the address of a byte buffer at an odd offset; constness is not established |
| `GetCollisionId` returns `EClsnId` | `SetCollisionId` takes it; the default returns -1 |
| `IsDrawEnabled` and the `IParticleSystem` flag getters return `unsigned int` | Accepted overrides' normalized ABI results |
| `int` for `Constrain`, `Attach`, `Detach`, both bounding-volume getters, `IsVisible`, `GetScriptObject`, `GetTeam`, `GetPlayerIndex` | ABI placeholders: the defaults return constants, and the overrides examined so far do not establish a type (`GetTeam` and `GetPlayerIndex` read bytes; `GetScriptObject` reads a pointer) |
| `void` | Every other slot |

Callers that use a placeholder result need evidence before its declaration is
changed.

`CScene` is a member-only view declaring `Remove(ISceneNode &)` for the external
`g_scene` object.

## Verification

ProDG 3.8.1 compiles virtual calls through the declared order at the original
offsets: `Stop` calls slot 91 (`+0x2d8`), `SetLocalToWorld` slot 71 (`+0x238`),
`Terminate` slot 1 and `Destroy`'s deleting destructor slot 2. Any missing or
misplaced declaration before those slots would shift those offsets. All particle
fragments accepted before the hierarchy was introduced still produce identical
objects, and the complete rebuilt analysis image is identical.

## Accepted defaults

`src/scene/` reconstructs 81 base-class functions (632 bytes) in seven fragments:

| Manifest | Range | Functions | Bytes |
| --- | --- | ---: | ---: |
| `scene_node_destructor` | `0x8027913c` | 1 | 48 |
| `scene_node_update_defaults` | `0x8027916c`-`0x802791e0` | 15 | 116 |
| `scene_node_centroid` | `0x80279220` | 1 | 52 |
| `scene_node_query_defaults` | `0x802792b4`-`0x802793ec` | 42 | 312 |
| `moving_node_light_defaults` | `0x802797fc`-`0x8027982c` | 8 | 48 |
| `moving_node_transform_defaults` | `0x80279950`-`0x80279984` | 13 | 52 |
| `moving_node_reset` | `0x8027ab08` | 1 | 4 |

Most defaults are empty or return a constant: `Constrain` returns 1,
`GetCollisionId` returns -1, and the casts and other getters return null.
`IMovingSceneNode::AsMovingNode` returns `this`. `GetCentroid` forwards to virtual
`GetPosition` (slot 21). `GetTMLocalToWorld` calls the accepted
`CMatrix::GetSlot(0)` on its output. The destructor stores `_vt.10ISceneNode` and
calls `IObserver`'s destructor; the table stays external original data.

## Next work

`GetPosition`, `GetRightward`, `GetForward`, `GetUpward` and
`GetWorldLinearVelocity` write constant vectors and wait on `CVector3` evidence.
`GetWorldLinearVelocity` returns its vector by value and stores four words: three
copies of one constant at `+0`, `+4` and `+8`, and a second constant at `+12`.
That suggests a 16-byte vector, but its type and fourth member are unconfirmed.
The slot map applies to every class derived from `IMovingSceneNode`; each derived
table should be checked against it rather than assumed.
