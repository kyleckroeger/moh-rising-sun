# PS2 research: particle recipe fields

Research notes from the PlayStation 2 release, the second topic offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). They complement
[ParticleRecipes.md](../../ParticleRecipes.md), which independently established the
GameCube getter indices. They come from kyleckroeger's own investigation for a separate
remake project, were written with AI assistance (Claude), and were re-checked on
2026-10-04 as described below. No game files or bulk dumps are included; the few values
quoted are short examples.

Nothing here is reconstructed source and nothing earns progress credit.

## Build, files and labels

- **PS2 release:** SLUS-20753. The program is `MOH3RDVD.ELF` (2,692,080 bytes, SHA-1
  `2d524fe5a0c9c452da60635e906670d30c556d63`), and **file offset = address − `0x135400`**.
  The level data is `levelfile.lfc` inside `DATA/1/1_1/LEVEL.VIV` and
  `DATA/1/1_11/LEVEL.VIV`; only these two levels were examined.
- **GameCube comparison:** the pinned GR8E69 `MOH3RDVD.ELF` (digests checked against
  `config/GR8E69/target.json`), disassembled with the repository's
  `powerpc-eabi-objdump`, and upstream `docs/ParticleRecipes.md` at `a701671`.
- **Sources:** **[notes]** are the contributor's earlier research notes (2026-10-03/04).
  **[re-check]** means re-done for this document with small read-only scripts.
  **[GC]** means read from GR8E69 disassembly for this document. **[upstream]** is this
  repository's own findings.
- **Confidence:** **high** means checked in the bytes and consistent across both levels,
  **medium** means consistent but with an untested alternative, and **low** means an
  interpretation.

## Where recipes live (PS2)

[notes] [re-check], confidence **high**: `levelfile.lfc` starts with four words: 2, a
size, the entry count (1,082 in `1_1`, 1,488 in `1_11`) and 16. From `0x10`, each entry
has three words: an item count, a length *L* in words, and an offset. At that offset are
*L* pointers to **12-byte items**, each made of a kind, a count and a value:

| Kind | Value |
|---|---|
| 0 | pointer to a file name |
| 1 | integer |
| 2 | float |
| 3 | CRC-32 of a name |
| 4 | a list of `count` strings |
| `0xFFFFFFFF` | empty |

**Particle recipes** are the entries with *L* = 50: there are 131 in `1_1` and 135 in
`1_11` [re-check]. In every one, item 0 is a kind-3 CRC: the **exact-case** CRC-32 of the
effect name, as in `zlib.crc32(b"Flo_showerhead01")`. That is the same exact-case
convention as FlexProp setting names. Using effect names
found in the level's own files, 120 of the 131 entries in `1_1` can be named (108
distinct names; some names have two entries) and 121 of 135 in `1_11`. No entry matches
only when the name is upper-cased. Names start with `Flo_` (continuous), `Pls_` (one-off
bursts) or `mPls_` (mesh pieces). Three entries per level store floats in fields that
are integers everywhere else. They are probably the three `mPls_` mesh-particle recipes
[notes], confidence **medium**.

GameCube status: the getter chain in [upstream] goes from the system's definition
(`+0x140`) to contents (`+0x04`) to the entry-pointer array (`+0x08`), and each entry's
value is at `+0x08`. That **matches** the PS2 12-byte item, whose value is its third
word. **Unverified:** the GameCube `.lfc` files, and whether GR8E69 finds recipes by the
CRC of the effect name.

## Field map

Indices are zero-based item positions, the same numbering as the [upstream] table.
"Kind" is what the PS2 data stores [re-check]. In every row where GameCube has a getter,
the GR8E69 load type (integer or float) agrees with the PS2 kind.

| Index | PS2 kind | Meaning | Confidence | GameCube status |
|---|---|---|---|---|
| 0 | CRC | exact-case CRC-32 of the effect name | high [re-check] | unverified |
| 1 | empty or int | unknown | — | unverified |
| 2 | file name | **sprite** image (for example `Liq_water02.ssh`) | high [notes] [re-check] | not read through the recipe-index chain; unverified |
| 3 / 4 | int | emission rate / emission delay | high | checked [upstream] |
| 5 | float | emitter (system) lifetime; 0 probably means unlimited | high (role); low (0) | checked [upstream] |
| 6 | int (some values look like float bit patterns) | unknown; earlier guess "random seed" is **not** supported. GR8E69 keeps the seed on the system object (`+0x160`) [upstream] | low | unverified |
| 7–9 / 10–12 | float | spawn box min / max | high | checked [upstream] |
| 13–15 / 16–18 | float | initial velocity min / max | high | checked [upstream] |
| 19–21 | float | **acceleration** (gravity appears as negative z, down to about −28) | medium [notes] | **consistent** [GC]: read by `CParticleSystem::Init`, `Reload` and the transform functions (`Move`, `Rotate`, `SetPosition`, `Transform`, …). That fits a vector copied into the system and turned with it. `GetParticleAcceleration` itself returns a cached copy at system `+0xf0`. |
| 22–24 / 39–41 | int | **start colour**, two copies (the second is probably a random range) | high (order); medium (meaning) | checked [upstream] [GC]: first and second outputs of `GetParticleColor`, low byte of each integer |
| 26–28 / 42–44 | int | **end colour**, two copies | as above | checked: third and fourth outputs |
| 25 / 45 / 29 | int | **alpha start / middle / end** (0–255) | high (order) | checked [upstream] [GC]: `GetParticleAlpha` outputs in that order |
| 30 | int | render type: **0 additive** (glows: fire, sparks, flashes), **2 normal blend** (smoke, steam, water). `1_1` has exactly 60 of each; `1_11` has 57 and 64 | medium (meaning, from how the effects look) | values unverified; getter checked [upstream] |
| 31, 33 / 32, 34 | float | start width, height / end width, height | high | checked [upstream] |
| 35 | float | third `GetParticleSize` output; role unknown | — | checked (index) [upstream] |
| 36 | float | particle lifetime, seconds (shower about 0.64) | high | checked [upstream] |
| 37 / 38 | float | spin min / max | medium (meaning) | index checked [upstream]; index 37 is also read by `Init` [GC] |
| 46 | int | 0 = fog applies (showers, steam); 1 = no fog (fire, sparks, smoke) | high (inversion); medium (examples) | checked [upstream] |
| 47 / 48 | float or empty | probably **fade**: fractions of a particle's life where fade-in ends and fade-out starts (for example 0.4 / 0.5); the PS2 set-up raises a flag when both are non-zero | medium | **consistent** [GC]: read by `GetParticleFade` and by `Init`; [upstream] has a separate `UseFade` bit |
| 49 | float or empty | probably a **draw-order offset** added to depth before sorting (fire about −25) | medium [notes] | **consistent** [GC]: the only reader found is `CParticleSystem::Draw` |

How the GC column was produced for 19–21, 37, 47–49 [GC]: a small script scanned the
GR8E69 particle code (`0x8005a000`–`0x80066000`) for the load sequence system `+0x140` →
`+0x04` → `+0x08` → entry *K*. It is a register-tracking heuristic, so its results count
as **consistent**, not as reconstructed proof. Items 2 and 6 were not found this way.

**Colour scale** [notes], confidence **low**: on PS2, 128 probably means normal brightness
and 255 twice as bright, which is the usual PS2 convention. GR8E69 copies the same low
byte into a colour with alpha `0xff` [upstream]. Whether GameCube rendering treats 128 the
same way is **unverified**, and it may **differ**.

**Empty items** [re-check]: indices 1, 47 and 49 are often the empty kind (`0xFFFFFFFF`).
How either program reads an empty item through a float getter was not examined.

## PS2 code locations

[notes], confidence **medium**; not re-checked for this document, because no MIPS
disassembler was used. The PS2 particle getters sit together at addresses
`0x15E5B0`–`0x15EA68` in `MOH3RDVD.ELF`, in the same order as the GR8E69 getters
(`GetSystemInitialVelocity` … `GetFogEnable`, `0x800610d0`–`0x80061598`). Their argument shapes
match too: one float, two floats, three floats, four colours, two vectors and a float.
The names come from the GR8E69 symbols; the original misspelling
`GetEmmisionRate` / `GetEmmisionDelay` appears there.

## Open questions

- The roles of items 1, 6 and 35, and how empty items behave.
- The render-type enum and colour scale on GameCube.
- The 11 to 14 unnamed recipe entries per level.
- The mesh-particle (`mPls_`) and trail (`eFlo_`, 44 items) recipe layouts.
- Recipe lookup by name CRC on GameCube.
