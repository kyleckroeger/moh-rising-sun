# Light-volume manager

`src/lighting/` reconstructs seven `CLightVolumeManager` methods (752 executable
bytes) from the pinned GR8E69 executable. Claude Code assisted the analysis and
verification. The [BPD evidence](BPD.md) supplies the light-volume prefix used here.

## Accepted fragments

| Manifest | Functions | Range | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `light_volume_manager` | constructor, destructor, `Reset`, `Update`, `AddVolume`, `RemoveVolume` | `0x80131758`-`0x801319c0` | 616 | 12 bytes at `0x802a7a64` |
| `light_volume_transition` | `UpdateTransition` | `0x80131dbc` | 136 | 68-byte message at `0x802a7a78` |

`GetVolume` (1,020 bytes) lies between them and remains original context; see below.

## Storage

The methods access seven words. `include/game/LightVolumeManager.h` names them
descriptively: four volume pointers, the outgoing volume, the remaining transition
time and the transition duration. The manager is embedded in objects (`CAnimObject`
at `+676`, `CStaticObject` at `+704`); `CStaticObject::SetLightVolumeTransitionDuration`
stores at `+728`, which is the duration word. The complete size is not established.
`CLight` is declared only for its static `GetDefaultLightVolume`.

## Behavior

- **Reset** clears the four slots and the outgoing volume, sets the remaining time
  to 0 and the duration to 10.0. The constructor and destructor call it.
- **AddVolume** does nothing when the fourth slot is occupied. Otherwise it inserts
  the volume before the first slot whose `priority` (`BPDLightVolume` `+0x28`) is
  lower, so higher priorities come first and equal priorities keep their arrival
  order. The shift uses `copy_backward` from the project's STLport subset; its
  guarded `memmove` matches the original exactly. If the first slot changed, it
  calls `UpdateTransition` with the previous first volume.
- **RemoveVolume** clears the first matching slot and shifts later volumes down,
  then calls `UpdateTransition` if the first slot changed.
- **UpdateTransition(outgoing)**: without a transition in progress it sets the
  remaining time to the duration. During a transition it prints the original
  "Still transitioning" message; if the volume being faded out is the new first
  volume, the remaining time becomes `duration - remaining` (the fade reverses);
  otherwise the remaining time is kept. The outgoing volume becomes the argument, or
  `CLight::GetDefaultLightVolume()` when it is null.
- **Update(step)** subtracts the step from the remaining time while a transition is
  active and ends it once the result is not greater than zero. Objects pass their
  `BeginUpdate` step, which is measured in ticks, so the default duration is ten
  ticks. `BIFunc_SetLightVolumeTransitionDuration` passes its script argument
  through unconverted.

## `GetVolume` (not reconstructed)

Read from the original instructions only:

- Without a transition it returns the first volume, or the default light volume.
- During a transition it fills a static output volume at `0x802c4d08` whose light
  records start at `0x802c4c48`. Light records are 48 bytes. The fields read are a
  type word at `+0`, a direction at `+16`-`+24`, a color at `+28`-`+36` and an
  intensity at `+40`; the light count and pointer are the volume's words at `+0x20`
  and `+0x24`. Type 1 lights are combined into output record 0; other lights use
  output record `index + 1`. Output intensity is 1.0, with the color scaled.
- The outgoing volume's lights are scaled by `intensity * remaining / duration`
  and the current volume's by `intensity * (1 - remaining / duration)`. Colors in
  an occupied output record are added. For a non-type-1 light in an occupied
  record, the direction moves from the earlier direction toward the new one by the
  current weight and is normalized with `sqrtf`.
- The direction blend builds a four-float temporary whose last word is 1.0. It
  waits on the `CVector3` evidence described in [Matrix.md](Matrix.md).

Type values other than 1, the output volume's full layout and the default volume's
contents are not established here.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces both
fragments, including the constants and message. The complete rebuilt analysis
image is identical. No instructions are patched, and no generated code or data is
discarded. Runtime behavior has not been tested.
