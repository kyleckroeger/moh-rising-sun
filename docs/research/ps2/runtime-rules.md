# PS2 research: update ticks and runtime rules (motion, lights, particles, camera, scenes)

Research notes from the PlayStation 2 release, following the six topics offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). Unlike the
earlier notes, most of this one was read from the GameCube program: the questions came
from a PS2 remake (what unit is a timer in? when does a motion loop?), and the answers
were found in GR8E69 functions, then matched against the PS2 level data.

These notes come from kyleckroeger's own investigation for a separate remake project,
were written with AI assistance (Claude), and were re-checked on 2026-10-05 as described
below. No game files or bulk dumps are included. Nothing here is reconstructed source or
earns progress credit.

## Build, files and labels

- **PS2 release:** SLUS-20753, with the program `MOH3RDVD.ELF` (SHA-1
  `2d524fe5a0c9c452da60635e906670d30c556d63`). The data examined is level 1_1 in
  `DATA/1/1_1/LEVEL.VIV`: the gameplay data `1_1_P.bpd`, the 45 `.mvd` motions, and the
  behaviour scripts `AnimatedMovingMech`, `AnimatedCameraMech`, `FireTrigger`,
  `TriggerWithChild`, `Action_PlayAnimation` and `NPC` (`.cbs`). Words are little-endian.
- **GameCube:** the pinned GR8E69 `MOH3RDVD.ELF`, read with its symbols and
  `powerpc-eabi-objdump`. Constants were read from the ELF's loaded segments.
- **Sources:** **[notes]** are the contributor's research notes of 2026-10-05.
  **[re-check]** means re-done for this document with read-only scripts over the PS2
  files. **[GC]** means read from GR8E69 for this document. **[remake]** means the rule
  was applied in the PS2 remake and the result compared by eye with the original PS2
  game; that is supporting evidence only.
- **Confidence:** **high** means read in the code or bytes, **medium** means consistent
  with the code but not fully traced, and **low** means an interpretation.

## 1. The update step is counted in video fields

[GC], confidence **high**.

| Step | Function | Address |
|---|---|---|
| Wait for the frame; step = 1.0 × fields waited | `WaitScreen()` → `CScreenEAGL::Wait()` → `VIGetRetraceCount` | `0x8016c650`, `0x8005c01c` |
| Store the step (multiplier 1.0 at `0x802c1b84`) | global at `0x802c51e8` (reset to 1.0 by `ClearTimer`) | |
| Pass it on (0.0 while a screenshot is taken) | `Gameloop_Update(CStaticScreen*)` | `0x8016c1e8` |
| Clamp to at most **6.0** (`0x802a54b4`) | `BSUtilUpdate(float)` | `0x800f8a2c` |
| Advance the script clock, fire due timers | `BSUpdateTimer(float)` | `0x8011dee8` |

So one tick is one TV field, **1/59.94 s** on NTSC, and the scripts' clock steps by whole
fields (at most 6, a tenth of a second). The matching `BSRegisterTimerEvent` stores
`g_CurrentTime + delay`, so a script timer's delay is in these ticks.
`CParticleSystem` uses the field rate directly: **59.94** at `0x802c1b88` is loaded by
`BeginUpdate`, `Render` and `UpdateBoundingBox`.

## 2. Motions: speed and looping

**Ticks per frame** [GC] [re-check], confidence **high**. The `.mvd` header word at byte 4
is a frame time in ticks: `CStaticObjectFactory::AllocateStaticObject` (`0x800cc91c`)
passes it to `CStaticObject::InitMotionProperties` (`0x800a612c`), which stores it at
`+612`; `CStaticObject::BeginAnimMotionUpdate(float)` (`0x800a6724`) reads `+612` and adds
the step to a counter until it reaches it [notes]. In 1_1, 44 of the 45 `.mvd` files have
2 (30 frames a second); `1_1_start_camera.mvd` has **4** (15 a second): its 87 frames
take 86 × 4 ticks, **5.74 s** [re-check]. [remake]: playing the start camera at this
speed matches the original.

**Looping** [notes], confidence **high** for the rule; [GC] re-checked the wrap.
`CStaticObject::StartMotionPlayback(int, int, EBSEventEnum)` (`0x800a9170`) keeps an end
frame. At the end frame `BeginAnimMotionUpdate` sends the event (if any), clears the
playing flag and holds the last frame. With end frame **0** it instead wraps the frame
(`divw`/`mullw` at `0x800a68a0`), so the motion loops for ever. The PS2
`AnimatedMovingMech` script reads its `Behavior` setting: 3 →
`StartMotionPlayback(-1, 0, 0, 117, …)` (loops); otherwise the end is `0x7fffffff`
(once, clamped to the frame count). Behaviors 1 and 2 also have their own handling when
stopped.

`Behavior` is at byte 148 of the PS2 `AnimatedMovingMech` record [re-check]. In 1_1:

| Behavior | Objects | Count |
|---|---|---|
| 3 (loops) | `a_mech_1_1_plane_card_mass01/02/03`, `ac_a_mech_plane_card_mass_ass/01/02`, `ac_a_mech_1_1_Kate_f_wall01/02/03` | 9 |
| 0 (once) | `plane_card_mass04/05/06`, the six `CC_…plane_card_mass07…` cards, the Japanese bomb, the two shellshock Kate pieces, `lifeboat_top_ outside`, `a_mech_Tuner_opneing_attackl` | 14 |
| 1 (once) | the chest lid, the two `TCAnimMovMech` hatches, three lifeboat parts, the burst pipe | 7 |

## 3. Animated lights: one colour frame per tick

[GC], confidence **high** for the fields read; [notes] for the timing.
`CPropertyAnimLight::BeginUpdate(float)` (`0x80130d88`) reads only three fields of the
light's `MOH_animatedLight_Struct`:

- the byte at **`+0x70`**, compared with 0, 1 and 2: the play mode, **0** once (then
  switched off), **1** loop, **2** back and forth;
- the halfword at **`+0x72`**: the frame count;
- the colour array at **`+0x74`**.

It advances one colour frame per tick (the step converted to whole frames) [notes]. It
does **not** read the float array at `+0x78`. In 1_1 those floats are the same for every
frame of a light (0.5, 1.0 or 2.5), so they are probably not frame times; a reach or
strength is more likely, confidence **low**.

> **Correction to the [`.bpd` notes](bpd-format.md):** those described `+0x78` as "frame
> times" with unclear units, and `include/game/BPD.h` names the field `times` following
> that. The update code gives no support for that meaning. A neutral name such as
> `field78` may be safer until a reader of the array is found.

Example from 1_1 [notes]: light ID 1 (the hallway alarm) is mode 1, 30 frames, red fading
to off, so it flashes twice a second at 60 ticks a second.

## 4. Particles: emitter lifetime and growth

**Emitter lifetime, recipe entry 5** [notes], confidence **high**; [GC] re-checked the
constants. `CParticleSystem::BeginUpdate(float)` (`0x80061a08`) compares
`GetSystemLifetime()` (`0x80061198`, matching source in `src/particles/ParticleScalars.cpp`)
with 0.0 (`0x80299cf0`). If it is not 0 and the current tick ≥ start + lifetime × 59.94,
the system is switched off: it stops emitting and its particles live out their lives.
So the lifetime is in **seconds** and **0 means for ever**. In 1_1 every one-off (`Pls_…`)
recipe has a short lifetime (0.02–1.87 s) and emits every tick.

**Growth, recipe entry 35** [notes], confidence **high**; [GC] re-checked the constants.
In `CParticleSystem::Render(CDrawContext&)` (`0x8005f8d8`) the size factor is
`(1 + (growth − 1) × u) × 0.5` with `u` = age ÷ particle lifetime (1.0 at `0x80299c60`,
0.5 at `0x80299c68`), applied to the camera's right and up vectors from the particle's
centre. The 0.5 makes it a half-width.

## 5. Field of view

[GC], confidence **high**. `CCamera::SetPerspective(float, float)` (`0x8012c4dc`) takes
the horizontal and vertical **half-angles** in radians and calls `tanf` on each.
`CPlayerObject::CommitUpdate` (`0x800b1808`) passes `h = FOV × π/180` (`0x8029f138`) and
`v = h × 0.75` (`0x802a764c`). `Gameloop_LoadLevel_Fixup_Phase07` (`0x80169d30`) starts a
level with FOV **35.0** (`0x8029ee60`; the same constant is loaded by
`CPlayerWeaponObject::DrawWeapon`). So the player sees **70° across and 52.5° top to
bottom**. Scripts change it with `SetGameFOV` (`0x8010216c`). [remake]: a 52.5° vertical
view matches the original's first frame.

## 6. Scene animations: switch at once, blend 0.25 s

[notes], confidence **high** for steps 1–3 and **medium** for step 4; [GC] re-checked the
blend constant.

1. The PS2 `Action_PlayAnimation` script sends event **288** with its `anim` setting to
   the NPC. With its flag `0x0f0f4590` set it waits for event **289** back; otherwise
   it finishes at once.
2. In the NPC script's second state table, states 100 and 112 store the sender and call
   `SetAnimationState(anim, 289)`. When the animation system raises 289, it is passed
   back to the sender. Other states answer 289 at once without playing.
3. `CSoldierObject::SetAnimationState(const char*, int, Partition)` (`0x800c1c2c`) calls
   `EALA::Character::Choreographer::SetState` (`0x8008055c`) with a blend time of
   **0.25 s** (`0x8029ff18`, loaded only by this function).
4. `SetState` picks the partition's running state by the order value at `+0x24` (as in
   [GameAlgorithms](../../GameAlgorithms.md)) and switches **at once**, not at a cycle's
   end. It compares the current and new tasks' core poses (`Task::GetCorePose`,
   `0x8008fc78`). If they differ (and neither is 99), it builds a sequence through an
   authored transition (`State::FindAuthoredTransition`, `0x800851ac`).
5. `CAnimObject::OnCycleEnd` (`0x800a2794`) only raises script event 27 on the object; it
   does not queue animations.

Not yet found: where an `.aso` state stores its core pose and authored transitions.

## 7. Behaviour scripts: fires and the cutscene camera

PS2 scripts [notes], confidence **high** for the order of calls.

**`FireTrigger`.** While burning (state 1): `CreateParticleSystemByName` for
`AliveParticles1/2` at the fire's own position, `CreateLightByType(0xf8eca6f1, 0, 0.3,
0, 0)` and a looping sound (`0xe88edb01`). On hit (event 40), if the hit's field 3 is
**28**, `FireHealth` goes down by 1, and at 0 the fire goes out. Messages 390 and 391
set and clear a flag that ignores hits. Put out (state 2): message 28 to its group,
removes the light, effects and sound, plays `0x18308a5b`, creates `DeathParticles1/2`,
and starts a timer of **`DeathTimer` ticks** (passed as is to `RegisterTimerEvent`, so
in the 1/59.94 s ticks of section 1, confidence **high**). When it fires (event 83) the
fire is removed. Message 52 puts it out the same way; 43 and 127 remove it at once.
Interpretations, confidence **low**: 0.3 as the light's strength, damage type 28 as the
fire extinguisher, `0x18308a5b` as the hiss.

**`TriggerWithChild`** (the parent class): event 136 sends `DamagePerHit` damage to
whoever is inside (the player only if not invincible), repeated every `DamageDelay`.

**`AnimatedCameraMech`.** On start: `AttachPlayerToProp` (`0x8010f834`), so the player
rides the camera along its motion, and `PlayerCanCrouch(0)`. Only if `UseMovieCamera`
is set: `SetGameFOV(MovieFOV)`, `ChangeClipPlane(MovieClipPlane)` and a wider letterbox.
On the end: `DetachPlayerFromProp` (`0x8010fa9c`). The player stays where the camera
ended and is **not** moved to a start point. With `UseMovieCamera`, the FOV goes back to
35 and the clip plane to 0.4. All four cameras in 1_1 have `UseMovieCamera` off.
[remake]: letting the player go where the start camera ends matches the original.

## Open questions

- The meaning of the `+0x78` floats of animated lights, and whether anything reads them.
- How animated lights and light volumes reach surfaces and characters (the rendering
  side of `CAnimLightManager` and `EnterLightVolume` was not read).
- Where an `.aso` state keeps its core pose and authored transitions.
- Where the attached player's eyes sit relative to an `AnimatedCameraMech`'s frames.
- Whether the GameCube scripts and level data match the PS2 ones read here (only the
  program code was compared).
