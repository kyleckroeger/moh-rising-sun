# PS2 research: `.bpd` gameplay layout files

Research notes from the PlayStation 2 release, the fifth topic offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). Each level's
`<level>_P.bpd` holds its gameplay objects (NPCs, triggers, effects, scripted steps,
moving props), with the class layouts, lighting zones, animated lights and AI navigation
data. This note covers the header, the sections, lighting and the object records.
Setting-name CRCs and the AI navigation sections are separate notes.

These notes come from kyleckroeger's own investigation for a separate remake project,
were written with AI assistance (Claude), and were re-checked on 2026-10-04 as described
below. No game files or bulk dumps are included; a few short example values are quoted.
Nothing here is reconstructed source or earns progress credit.

## Build, files and labels

- **PS2 release:** SLUS-20753, with the program `MOH3RDVD.ELF` (SHA-1
  `2d524fe5a0c9c452da60635e906670d30c556d63`). The data examined is `1_1_P.bpd` in
  `DATA/1/1_1/LEVEL.VIV` and `1_11_P.bpd` in `DATA/1/1_11/LEVEL.VIV`. PS2 file words are
  little-endian, and offsets inside the file are from the file's start.
- **GameCube comparison:** the pinned GR8E69 `MOH3RDVD.ELF` (digests checked against
  `config/GR8E69/target.json`), using symbols and `powerpc-eabi-objdump`. The main
  functions read are `EndianSwap(BPDHeader&)` (`0x80123538`), `EndianSwap(BPDLightVolume&)`
  (`0x801238ec`) and `AIPathFinding::ImportAreaPathNodeNetwork(BPDHeader*)`
  (`0x800e7320`). The GameCube `.bpd` files themselves were **not** examined.
- **Sources:** **[notes]** are the contributor's earlier research notes (2026-10-02/04).
  **[re-check]** means re-done for this document with read-only scripts. **[GC]** means
  read from GR8E69 for this document.
- **Confidence:** **high** means checked in the bytes in both levels, **medium** means
  consistent but not exhaustively traced, and **low** means an interpretation.

## Header

[re-check] [GC], confidence **high** for positions and GameCube types. The header is
`0x70` bytes. At `0x10`–`0x6F` are twelve (offset, count) pairs, except `0x50`, which is
(count, offset). GR8E69's byte-swap routine names the type of each pointer, so the
GameCube types in the table below come from its `ChangeEndian<T*>` calls.

| Header | PS2 `1_1` | PS2 `1_11` | GR8E69 type | Contents (PS2) |
|---|---|---|---|---|
| `0x00` | 13 | 13 | int | unknown (earlier guess: section count) |
| `0x04` | file size + 2 | file size + 2 | int | about the file size (the same +2 in both levels) |
| `0x08` | 101 | 101 | int | **unknown**. The earlier notes read this as the plane count, which is **wrong**: see "Planes and lighting zones" |
| `0x0C` | 0 | 0 | int | — |
| `0x10` | `0x51F8`, 24 | `0xC10`, 11 | `BPDPolyPath*` | NPC paths (Pathfinder note) |
| `0x18` | 0, 0 | 0, 0 | `BPDPathFindingNode*` | empty in both PS2 levels |
| `0x20` | 0, 0 | 0, 0 | `void**` | empty in both PS2 levels |
| `0x28` | `0x47B4`, 7 | `0xAA4`, 1 | `void*` | placed animated lights |
| `0x30` | `0x4450`, 7 | `0x740`, 7 | `void*` | animated light patterns |
| `0x38` | `0x33C0`, 59 | `0x190`, 1 | `BPDLightVolume*` | lighting zones |
| `0x40` | `0x26E58`, 92 | `0x29B80`, 54 | `void*` | class layouts |
| `0x48` | `0x6ACC`, 1,096 | `0xE50`, 1,235 | `void*` | object records |
| `0x50` | 1,524, `0x28CD8` | 1,575, `0x2AFB4` | int, then `void*` | string table: a count, then the offset |
| `0x58` | `0x5C6C`, 42 | 0, 0 | `BPDPathFindingArea*` | walkable areas (Pathfinder note) |
| `0x60` | `0x5774`, 106 | 0, 0 | `PropVec3*` | area corner points (Pathfinder note) |
| `0x68` | `0x6AC0`, 1 | 0, 0 | `BPDPathFindingBSP*` | area BSP header (Pathfinder note) |

The string-table offset is the `0x54` word. In `1_1` the table ends with the text
`STRING TABLE END`, and searching for the table by its first string can hit a false match
[notes].

## Planes and lighting zones

**Lighting zones** [notes] [re-check] [GC], confidence **high** for the layout. Each zone
is 48 bytes:

| Offset | Contents | GR8E69 `BPDLightVolume` |
|---|---|---|
| `+0x00` | bounding box: min x, y, z, then max x, y, z (6 floats) | 6 floats |
| `+0x18` | plane count | int |
| `+0x1C` | offset of the zone's first plane | `PropPlane4*` |
| `+0x20` | light count | int |
| `+0x24` | offset of the zone's lights | `BPDLight*` |
| `+0x28` | priority | int |
| `+0x2C` | 0 in every zone | not swapped (probably filled at run time) |

[re-check]: every zone has exactly one type-1 light. The `1_1` priorities are 0 (22
zones), 1 (31), 2 (1), 3 (1) and 6 (4).

**Planes** [re-check] [GC]: the zones' planes are 16-byte `PropPlane4` records (a unit
direction x, y, z, then a distance) packed from `0x70`. In `1_1` the 59 zones reference
**359** distinct planes, filling `0x70`–`0x16E0` exactly; the first light record starts
at `0x16E0`. In `1_11` the single zone uses 6 planes. This settles the earlier open
question: the planes are the lighting zones' sides, and the header's 101 is not their
count. The earlier guesses that they are trigger or fog volumes are withdrawn.

**Lights** [notes], confidence **medium**. Each light is 48 bytes:

- the type: 1 is ambient (exactly one per zone, re-checked); 2 is directional (1 to 3
  per zone);
- a position (meaningless for ambient lights);
- a unit direction;
- a colour (r, g, b in the range 0–1);
- a strength (0.1–1.4);
- 0.

GameCube status: the `BPDLight` field layout is **unverified**. GR8E69 symbols show that
moving objects, the player and thrown objects enter and leave light volumes
(`EnterLightVolume`, `IsPointInLightVolume`), and that a manager blends between them
(`CLightVolumeManager::UpdateTransition`, `SetLightVolumeTransitionDuration`). In the PS2
data, zones that overlap have priorities where the smaller, inner zone has the higher
number in 55 of 56 pairs. So the higher priority **probably** wins [notes], confidence
**low**.

**Animated lights** [notes], confidence **medium** for the layout and **low** for the
units. Pattern records (`0x30`) have their own size and begin with a type word
`0x2C3C203F`. At byte 112 they hold the frame count (high 16 bits) and a mode (0/1/2),
then offsets of the frame colours, packed `0x00BBGGRR`, and the frame times. Placed lights
(`0x28`) use the same record with a position at byte 16, an ID at byte 56 (the number
`AnimatedLightControlTrigger` switches) and inline frames. The time unit (seconds, or 1/30
s) is unclear. GameCube status: **unverified**. GR8E69 symbols include
`MOH_animatedLight_Struct`, `CAnimLightManager`, `CPropertyAnimLight` and
`CInstancedAnimLight`.

## Object records

[notes] [re-check], confidence **high**. Records follow one another from the `0x48`
offset. Each record has:

- `+0x00` the name, as a string-table index;
- `+0x04` the class, as a string index;
- `+0x08` the record size, including this header;
- `+0x0C` a 3×3 rotation (9 floats);
- `+0x30` a position (x, y, z);
- from `+0x3C`, the class's settings, at the byte offsets given by the class layout.

[re-check]: stepping by record size, the 1,096 records in `1_1` and 1,235 in `1_11`
end exactly where the class-layout section begins. GameCube status:
**consistent**. Upstream `docs/FlexProp.md` places position floats at `0x30`/`0x34`/`0x38`
and a twelve-float transform from `0x0c`, and FlexProp data at a `0x3c` prefix plus
offset.

**Class layouts** (`0x40`): per class, a name, a data size, a parent, a setting count,
then 12-byte (CRC, type, offset) records sorted by signed CRC. These are covered in the
separate FlexProp setting-name note.

**Positions** use the same coordinate system as the level geometry [notes], confidence
**high**. For example, the two opening-scene NPCs stand side by side on the bunk room
floor.

## Triggers and events (PS2 interpretation)

All entries in this section are [notes], from reading the PS2 data together with
script behaviour. GameCube status is **unverified** throughout.

- **Trigger zones**, confidence **medium**. Every `…Trigger` class has, at record bytes
  60–87, a shape number followed by two groups of three floats. Shape 1 (and 0) is a
  **cylinder** using the first group: radius, then top and bottom relative to the
  position. The names are the settings `Radius`, `CylinderTop` and `CylinderBottom`.
  Shape 2 is a **box** using the second group: half-sizes along the object's own axes,
  where the sign does not matter. The unused group holds defaults (10, 2, −5). This was
  checked by casting rays against the collision for a few triggers. An earlier misreading
  (the middle value as a height) was corrected.
- **Event IDs**, confidence **medium**. Objects are linked by 32-bit IDs from `0x10000`
  to `0x1FFFFF`, stored as a count followed by the IDs. Triggers broadcast on IDs, and
  effects, NPCs and moving objects listen on them. The high part acts as a kind:
  `0x1xxxx` existence (create/kill), `0x4xxxx` explosions and objectives, `0x6xxxx`
  activate, `0x10xxxx` scripted scenes, `0x11xxxx` effects and `0x12xxxx` sounds. The
  same kinds appear as the ID kinds in `.sin` message handlers. The byte offset of the
  list's count is per class, for example `BasicTrigger` 116, `OneShotTrigger` 104,
  `GagTrigger` 120, `SpriteParticleSystem` 80, `NPC` 300 and `AnimatedPlaneMech` 240.
- **Message codes**, confidence **medium** for the first four. After a trigger's ID list
  come up to four short code lists (a count, then codes). Codes: 53 activate, 52
  deactivate, 4 create, 316 kill, 255 detonate, 43 remove (low), 126/127 die (low).
  Codes from 347 upward release a script's `WaitForGoCode` *N*, where *N* = code − 346
  (low).
- **Start rule**, confidence **medium**. Moving objects and props name one at byte 72:
  `StartOnInitialize`, `StartByIdObject` (waits to be created) or `CreateByExplosion`
  (appears when its group is detonated). The last explains the intact/damaged swaps.
- **`CompartmentTrigger`**, confidence **medium**. These 116-byte records list, at bytes
  96–111, the **four level chunks** to keep loaded, numbered from 0. In all 25 triggers
  in `1_1`, the list includes the chunk the trigger stands in.
- **Fog** (`FogParams`, `ClipAndFogTrigger`), confidence **medium**. A zone like a
  trigger's, then from byte 92: near distance, far distance and a colour (3 floats,
  0–255). The `FogParams` boxes are thin and stand in doorways. Byte 112 is not decoded.

## Open questions

- Header words `0x00` and `0x08`, and the empty `0x18`/`0x20` slots, which GameCube
  types as `BPDPathFindingNode*` and `void**`.
- The `BPDLight` fields on GameCube, the animated-light time unit and the mode values.
- The GameCube `.bpd` file bytes.
- Classes holding single IDs without a count (`FireTrigger`, `Objective_…`,
  `ShellshockControl`).
- The later message-code lists.
- `1_1_P.psp` (`PBSP_HEADER`), not examined.
