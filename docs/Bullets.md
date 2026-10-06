# Bullets

`src/bullets/` reconstructs 22 `CBullet` functions (520 executable bytes) from the
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

## Damage in the derived classes (not reconstructed)

`CProjectileBullet::GetDamage`/`SetDamage` read and write the float at `+76` of the
record that the bullet's word at `+0xc8` points to; `CThrownBullet` uses `+92` for
damage and `+96` for the blast radius. `CThrownBullet::SetExplosionParticleSystem`
stores the ID in a word array at `+612` indexed by the explosion type.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces both
fragments, including their constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.
