# PS2 research: `.sin` class descriptions (states, events, messages)

Research notes from the PlayStation 2 release, the fourth topic offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). Each behaviour-script
class has a `.cbs` code file and a `.sin` description. The `.sin` holds the class's text
constants and its **state machine**: the states, which events and messages each state
handles, and where the handler code starts. The instruction set is a separate topic.

These notes come from kyleckroeger's own investigation for a separate remake project,
were written with AI assistance (Claude), and were re-checked on 2026-10-04 as described
below. No game files, script text or bulk dumps are included. Nothing here is
reconstructed source or earns progress credit.

## Build, files and labels

- **PS2 release:** SLUS-20753. The program is `MOH3RDVD.ELF` (2,692,080 bytes, SHA-1
  `2d524fe5a0c9c452da60635e906670d30c556d63`). PS2 locations are **addresses**, and file
  offset = address − `0x135400`. The data examined is the `.sin` files in
  `DATA/1/1_1/LEVEL.VIV` (110) and `DATA/1/1_11/LEVEL.VIV` (68). PS2 file words are
  little-endian.
- **GameCube comparison:** the pinned GR8E69 `MOH3RDVD.ELF` (digests checked against
  `config/GR8E69/target.json`), using its symbols and `powerpc-eabi-objdump`.
- **Sources:** **[notes]** are the contributor's earlier research notes (2026-10-03/04).
  **[re-check]** means re-done for this document with read-only scripts. **[GC]** means
  read from GR8E69 for this document.
- **Confidence:** **high** means checked in the bytes across all files, **medium** means
  consistent but not exhaustively traced, and **low** means an interpretation.

## File layout (PS2)

[notes] [re-check]. The walk follows the PS2 loader at `0x1F9278` [notes].
[re-check], confidence **high**: all 178 files in both levels are consumed exactly to
their end by this walk, leaving at most zero padding.

| Where | Contents |
|---|---|
| `0x00` | `BSSCRIPTFILETAG`, as in `.cbs` |
| `0x6C` | class name |
| `0xAC` | CRC-32 of the class name, **exact case**. [re-check]: equal in all 178 files. |
| `0xBC` | number of class-level records (16-bit). [re-check]: 1 in 168 files, 2 in 5 and 3 in 5. This **corrects** the earlier notes, which said it is always 1. |
| `0xC0`, `0xC4` | number of text constants, and the offset of their offset table. The strings start at `0x108`, zero-terminated and padded to 4 bytes. |
| then | per level, a 28-byte record: `+0x04` code length in words; `+0x0C` **state count** (16-bit); `+0x14` **object-variable count**; `+0x18` class depth (byte). Then depth + 1 words follow, giving the code start of each level, filled in at load. |
| then | one word per state, saying whether its record is present (`0xFFFFFFFE` / `0xFFFFFFFF` mean no record here: the state is inherited from the parent class) |
| then | **state records**, 20 bytes each: `+0x00` entry code position; `+0x04` and `+0x08` lists (GR8E69 uses `+0x04` as the event-list pointer at run time; how it is filled was not traced); `+0x0C` **parent state** (16-bit, `0xFFFF` = top level); `+0x0E` a byte; `+0x0F` **message-handler count**; `+0x10` **event-handler count** |
| then | every state's **event handlers**, 8 bytes each |
| then | every state's **message handlers**, 12 bytes each |

**Code positions** are `(class depth << 24) | word offset`, where depth 0 is the root
class's code [notes], confidence **medium**.

GameCube status: **checked for the in-memory shapes** [GC], confidence **high**.
GR8E69 `BSFindAnyEventHandler(unsigned short, BSClass_struct*)` (`0x800f460c`) does the
following:

- reads a 16-bit level count from the class;
- steps through **28-byte** level records, reading a 16-bit **state count at `+0x0C`**;
- for each present state, reads a byte **event-handler count at `+0x10`** and the event
  list pointer at `+0x04`;
- steps through **8-byte** event entries, comparing the 16-bit value at entry `+0x04`
  with the requested event.

Those are the PS2 offsets. That the GameCube **file** bytes match was not examined; the
GameCube `.sin` files have not been checked.

## Handler entries

Fields are described as bytes and halfwords, which is the reading that agrees with both
the PS2 little-endian files and the GameCube big-endian comparison above. (The earlier
notes read them as whole words, such as `0x0301xxxx`.)

**Event handler** (8 bytes) [re-check] [GC], confidence **high** for the layout:

| Offset | Size | Contents |
|---|---|---|
| `+0` | 32 bits | code position of the handler |
| `+4` | 16 bits | **event number** (GR8E69 compares this halfword) |
| `+6` | byte | unknown |
| `+7` | byte | unknown |

The observed (`+6`, `+7`) pairs over 2,213 entries [re-check] are: (1, 3) 1,223; (1, 1)
817; (3, 1) 66; (0, 3) 35; (0, 1) 27; (0, 6) 18; (0, 2) 13; (4, 1) 10; (2, 1) 4. Their
meaning is **not decoded**.

**Message handler** (12 bytes) [notes] [re-check]:

| Offset | Size | Contents |
|---|---|---|
| `+0` | 32 bits | code position |
| `+4` | 16 bits | usually 1, sometimes 3, 4 or `0xFFFF` |
| `+6` | 16 bits | **ID kind** (see below) |
| `+8` | 16 bits | **message code** |
| `+10` | 16 bits | observed values: `0x0303` (378 of 598), `0x0300` (139), `0x0304` (25), `0x0103` (19), `0x0600` (18), `0x0101` (11), `0x0301` (6), `0x0100` (2) |

Interpretation [notes], confidence **medium** for the dominant form only: entries with
`+4` = 1 and `+10` = `0x0303` mean "message code *N*, arriving through an event ID of
kind *K*". The other forms are **not decoded**. GameCube status: **unverified**. GR8E69
has `BSRegisterMessage`, `BSRegisterMessageToEvent(unsigned short, unsigned short,
BSObject*)` and `BSSendMessage(unsigned short, …)`. Those signatures are consistent with
16-bit message codes and event IDs, but the bodies were not traced.

## What the numbers mean (PS2 script use)

All entries in this section come from reading how scripts use the numbers [notes]. They
are interpretations, and their GameCube status is **unverified**. 271 distinct event
numbers and 112 distinct message codes occur in the two levels [re-check].

**Events.** These are fairly sure (**medium**) because the engine sends them or the
classes' behaviour makes them clear:

| Event | Meaning |
|---|---|
| 41 | object created (sent by the creation code) |
| 260 / 261 / 262 / 263 / 264 | NPC enters / NPC leaves / player enters / player leaves / activated (`BasicTrigger`) |
| 117 | a motion finished (`StartMotionPlayback(…, 117, …)`) |
| 119 | a line or sound finished |
| 161 | the player came within a requested distance |
| 422 | save/load: each state stores its own number with `SerializeValue` and returns to it on load |

These are **low** confidence: 40 / 42 hit / destroyed; 114 / 115 on / off; 265 looked
at; 288 → 289 play-animation request → finished; 371 "fire" (`FiringTrigger_Timed`); 407
"die" (`KillTrigger`); 446 → 447 timer request → time up. Scripts also choose their own
timer event numbers with `RegisterTimerEvent`.

**ID kinds** (the `+6` field of a message handler, low confidence except where noted):

| Kind | Meaning |
|---|---|
| `0x1` | existence: create, kill |
| `0x4` | explosions and objectives |
| `0x6` | activate |
| `0xF` | scenes started by gag triggers |
| `0x10` | scripted scenes |
| `0x11` | effects |
| `0x12` | sounds |
| `0x2`, `0xA` | used by NPCs; not decoded |

**Message codes.** These are **medium** confidence, because their effects were seen:

| Code | Meaning |
|---|---|
| 4 | create |
| 52 / 53 | deactivate / activate |
| 255 | detonate (exploding props) |
| 316 | kill (moving objects) |

These are **low** confidence: 43 remove ("suicide"); 78 a torpedo hit, counted by one
objective; 126 plane shot down; 127 destroyed/killed; 326 / 327 sound on / off; 390 /
391 fire on / off. Many NPC codes are not decoded.

## Open questions

- The meaning of the event-entry bytes `+6`/`+7` and of the minority message-entry forms.
- The unnamed byte at state `+0x0E`.
- The GameCube `.sin` file bytes, including their byte order.
- The GameCube enum names for events and messages, if any survive (none were searched for).
- The NPC message codes.
