# Bullets

`src/bullets/` reconstructs 38 `CBullet`, `CProjectileBullet` and `CThrownBullet`
functions (672 executable bytes) from the
pinned GR8E69 executable. Claude Code assisted the analysis and verification.

## Declarations

The `CBullet` table at `0x802e9eb0` follows the [scene-node slot map](SceneNode.md)
for slots 1-88 and adds slots 89-102: `Init`, `Shutdown`, `GetUp`, `GetDamage`,
`GetScriptBulletType`, `GetFiredBy`, `AsThrown`, `AsProjectile`, `GetVelocity`,
`SetExclusionPair`, `Penetrate`, `SetDamage`, `SetBlastRadius` and
`SetExplosionParticleSystem`. `CProjectileBullet` (`0x802ea210`) adds
`SetDrawForward` and `CThrownBullet` (`0x802ea5a8`) adds `IsCollisionEnabled` at slot
103. `include/game/Bullet.h` declares `CBullet` under `#pragma interface` with a
scoped view: a collision ID at `+0x40` (`Shutdown` resets it to -1, and both derived
`GetCollisionId` overrides read it), the local-to-world matrix at `+0x70` (the axis
getters read its rows) and two floats at `+0xb0`/`+0xb4`. Return types follow the
same conventions as [SceneNode.md](SceneNode.md): `AsThrown`/`AsProjectile` return the
named classes; `GetFiredBy` returns `ISceneNode *`, inferred from its name and the
derived classes' pointer field; `GetScriptBulletType` is an `int` placeholder.

## Accepted fragments

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `bullet_base` | `Shutdown`, `Draw`, `BulletBaseDraw`, `IsDrawEnabled` | 80 | 8 bytes at `0x802a8c1c` |
| `bullet_defaults` | axis getters, `GetTMLocalToWorld`, `AsBullet`, and the base defaults of slots 92-102 | 440 | 20 bytes at `0x802a8c50` |

`Shutdown` sets `+0xb0` to 0, the collision ID to -1 and `+0xb4` to the largest
float, in that order. `Draw` calls the empty global `BulletBaseDraw`. The base
`GetDamage` returns 0, `GetScriptBulletType` -1, and the casts and `GetFiredBy`
null; the other defaults are empty.

## Derived accessors

`Init` stores its `ProjectileBulletProperties_struct` or `ThrownBulletProperties_struct`
argument at `+0xc8`; the scoped record views name only the fields read below.

| Manifest | Functions | Code bytes |
| --- | --- | ---: |
| `projectile_collision_id`, `thrown_collision_id` | `GetCollisionId` (both) return the collision ID at `+0x40` | 16 |
| `projectile_exclusion_pair` | `CProjectileBullet::SetExclusionPair` (empty) | 4 |
| `projectile_set_damage` | `SetDamage` writes record `+76` | 12 |
| `projectile_accessors` | `GetDamage` (record `+76`), `GetFiredBy` (`+0xe0`), `AsProjectile`, `SetDrawForward` (bit 28 of the word at `+0x10c`) | 40 |
| `thrown_setters` | `SetDamage` (record `+92`), `SetBlastRadius` (record `+96`), `SetExplosionParticleSystem` (word array at `+0x264` indexed by type) | 40 |
| `thrown_collision_light` | `IsCollisionEnabled` (returns 1), `GetAttachedLight` (`+0x254`) | 16 |
| `thrown_accessors` | `GetDamage` (record `+92`), `GetFiredBy` (`+0xe0`), `AsThrown` | 24 |

`+0xe0` is named from `GetFiredBy`; `Init` reads it as a pointer and stores its
`ISceneNode *` argument through it, so its exact type is not established. The
bounding-volume getters (embedded 80-byte volumes at `+0x110`/`+0x160` and
`+0x1a0`/`+0x1f0`) and `CThrownBullet::SetLightVolumeTransitionDuration` are not
reconstructed.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces both
fragments, including their constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.
