# PS2 research: Pathfinder interactive-music rules (`.MPF`)

Research notes from the PlayStation 2 release, the sixth topic offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). "Pathfinder" here
is EA's interactive-music system. It has nothing to do with AI path finding. Each level
has a small `<level>M.MPF` rules file that chooses which part of `MAIN.MUS` plays, and
what a music event switches to.

These notes come from kyleckroeger's own investigation for a separate remake project,
were written with AI assistance (Claude), and were re-checked on 2026-10-04 as described
below. No game files, audio or bulk dumps are included. Nothing here is reconstructed
source or earns progress credit.

## Build, files and labels

- **PS2 release:** SLUS-20753, with the program `MOH3RDVD.ELF` (SHA-1
  `2d524fe5a0c9c452da60635e906670d30c556d63`). The data examined is
  `DATA/1/1_1/SOUND/1_1M.MPF` (328 bytes), `DATA/1/1_11/SOUND/1_11M.MPF` (312 bytes) and
  the matching `MAIN.MUS` files. Words are little-endian.
- **GameCube comparison:** the pinned GR8E69 `MOH3RDVD.ELF` (digests checked against
  `config/GR8E69/target.json`), using symbols, strings and `powerpc-eabi-objdump`. The
  GameCube music files were **not** examined.
- **Sources:** **[notes]** are the contributor's earlier research notes (2026-10-04).
  **[re-check]** means re-done for this document with read-only scripts. **[GC]** means
  read from GR8E69 for this document.
- **External reference:** the field layout follows the public [vgmstream reader for
  this format](https://github.com/vgmstream/vgmstream/blob/314a0ccae14a1b63f52f422799ead622183d9106/src/meta/ea_schl_map_mpf_mus.c),
  version 3.2. It was used only as a guide; no code was copied.
- **Confidence:** **high** means checked in the bytes, **medium** means consistent with
  the files but not fully traced, and **low** means an interpretation.

## Program interface

**PS2** [notes]: the music functions are `MUSIC_Start`, `MUSIC_SendEvent`,
`MUSIC_SetLevel` and so on. Scripts reach them through the built-in functions
`PathfinderEvent`, `PathfinderSetLevel`, `PathfinderFadeVolume` and
`PathfinderSetLatency`.

**GameCube** [GC], confidence **high**. GR8E69 has the same interface:

| Script function | Address | Calls |
|---|---|---|
| `BIFunc_PathfinderEvent` | `0x80108508` | `MUSIC_SendEvent(int)` |
| `BIFunc_PathfinderSetLevel` | `0x8010848c` | `MUSIC_SetLevel(int)` |
| `BIFunc_PathfinderFadeVolume` | `0x80108584` | `MUSIC_Fade(float, int)` |
| `BIFunc_PathfinderSetLatency` | `0x80108638` | `MUSIC_SetLatency(int)` |

GR8E69 also has `MUSIC_Start(int, int, int)` (`0x80158f10`), `MUSIC_Stop`, `MUSIC_Pause`
and `MUSIC_SetVolume`. Its strings include the file paths
`data\%d\%d_%d\sound\%d_%dM.mpf` and `data\%d\%d_%d\sound\main.mus`, so the GameCube
version loads one rules file and one music file per level under the same names.

## File layout

[notes] [re-check] for the header and piece table, which are confidence **high**. The
nodes and routers are [notes], confidence **medium**.

| Offset | Contents | `1_1` | `1_11` |
|---|---|---|---|
| `0x00` | magic `xDFP` (`PFDx` stored backwards) | ✓ | ✓ |
| `0x04`, `0x05` | version, sub-version | 3, 2 | 3, 2 |
| `0x0D`–`0x11` (bytes) | tracks, sections, events, routers, variables | 1, 1, 6, 6, 0 | 1, 1, 6, 6, 0 |
| `0x12`–`0x13` (16-bit, little-endian) | node count (music steps) | 15 | 13 |
| `0x24` | node offsets: 16-bit, multiplied by 4 | 68, 84, 100, … | 64, 76, 92, … |

Maintainer cross-check (2026-10-04, Codex-assisted): the referenced reader uses a
16-bit read at `0x12` for the node count. This corrects the original note's byte-width
label; the reported PS2 values have not been independently reproduced here.

- **Nodes**, confidence **medium**: 12 bytes plus their links. Byte 11 is the link count.
  A link word `0x00NN7F00` means "go on to node NN", probably at volume `0x7F`. The first
  byte of a node is 0, 1, 2 or `0xFF` (empty), and its role is **not decoded**.
- **Events and routers**, confidence **medium**: after the nodes comes the event table,
  followed by one router per event. The high 16 bits of router *k* give the node that
  event *k* starts. In `1_1` these are 0, 4, 7, 9, 11 and 13.
- **Pieces** [re-check], confidence **high**: 8 bytes each, holding an offset in
  `MAIN.MUS` and a length in milliseconds. In `1_1` the table is at `0x138`:
  (0, 82,337 ms) and (`0xEFBA0`, 46,718 ms). `1_11` has three pieces (about 215, 73 and
  165 s) [notes].

## How `1_1` uses it (PS2 interpretation)

[notes], confidence **medium**; GameCube status **unverified**.

- **Event 0** starts node 0, which goes on to 2 and 3, then loops back to 2: a looping
  piece. **Event 1** starts node 4, then goes to 6 and 5. Events 2–5 start empty node
  pairs, probably meaning stop or silence.
- In the level, a single `PathFinderTrigger` (`PathFinderTrigger02_shellshock`) sends
  **event 0** once, when the player enters it at the shellshock scene, with the music at
  100 %. Despite its name, this trigger is about music, not navigation.
- The California-ejection objective script (`Objective_1_1_CaliforniaEject`) sends
  **event 1**. So the music starts at the shellshock and changes when the player is
  thrown overboard.

## Open questions

- The node fields: the first byte, and which part of a piece each node plays.
- The meaning of events 2–5.
- The GameCube `.MPF` and `MAIN.MUS` bytes, including byte order and audio codec.
- The `MUSIC_*` function bodies on GameCube, beyond the calls listed above.
