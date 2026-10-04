# PS2 research: behaviour-script instructions and built-in functions

Research notes from the PlayStation 2 release, the third topic offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). Every gameplay
class (`NPC`, `BasicTrigger`, `CameraShaker`, …) runs as compiled code for a small
stack-based script machine. This note covers the code files, the instruction set and the
built-in function (BIF) table. The `.sin` class descriptions (states, events, message
handlers) are left for a separate note.

These notes come from kyleckroeger's own investigation for a separate remake project,
were written with AI assistance (Claude), and were re-checked on 2026-10-04 as described
below. No game files, scripts or decompiled script text are included. Nothing here is
reconstructed source or earns progress credit.

## Build, files and labels

- **PS2 release:** SLUS-20753. The program is `MOH3RDVD.ELF` (2,692,080 bytes, SHA-1
  `2d524fe5a0c9c452da60635e906670d30c556d63`). All PS2 locations are **addresses**, and
  file offset = address − `0x135400`. The data examined is the `.cbs` files in
  `DATA/1/1_1/LEVEL.VIV` (110) and `DATA/1/1_11/LEVEL.VIV` (68), and `bsbifunc.dat`.
- **GameCube comparison:** the pinned GR8E69 `MOH3RDVD.ELF` (digests checked against
  `config/GR8E69/target.json`), using its symbol table and the repository's
  `powerpc-eabi-objdump`.
- **Sources:** **[notes]** are the contributor's earlier research notes (2026-10-03/04).
  **[re-check]** means re-done for this document with small read-only scripts. **[GC]**
  means read from GR8E69 for this document.
- **Confidence:** **high** means checked in the bytes, **medium** means consistent
  behaviour but not every case traced, and **low** means an interpretation.

## Code files (`.cbs`)

[notes], confidence **high**. Each file starts with the tag `BSSCRIPTFILETAG`. At `0x20`
is the version (30) and at `0x24` the **parent class name**. Classes inherit, and the
loader (PS2 `0x1f9520`) loads the parent first. At `0x64` is the function count, followed
by each function as a word count and that many 32-bit words. In the earlier notes, all
110 `1_1` files ended exactly after their last function. [re-check]: both levels parse as
178 files, 193 functions and 44,287 instructions.

GameCube status: **unverified** for the file layout. GR8E69 has `BSInitFile`
(`0x800f44c4`), but the GameCube script files were not examined.

## Instruction encoding

[notes] [re-check], confidence **high**. An instruction is one little-endian word on PS2:

- the **low byte** is the opcode;
- the next byte is a sub-value;
- the **high 16 bits** are the argument, signed for jumps.

`0x11` (PUSH) and `0x14` (FNCALL) take one extra word. [re-check]: decoded that way, every
opcode in all 178 files is in `0x00`–`0x20`. Opcode `0x1D` (GP) never appears in either
level, and `0x06` (NEG) does not appear in `1_11`.

The stack grows upward. Binary operations pop two values (the left operand is the lower
one) and push the result. Arithmetic instructions carry an operand type in their
argument: 2 means integer and 3 means float, and mixed pairs are converted [notes],
confidence **medium**.

## Opcode table

The numbering is **checked on GameCube** [GC], confidence **high**. GR8E69
`BSInitMachine` (`0x800f4dac`) stores each `BSOpCodeFunc_*` handler into
`g_bocfOpCodeFuncs` (`0x8031d078`) at slot *N* × 4. Tracing those stores gives the
GameCube number for all 33 handlers, and every one equals the PS2 number below. The PS2
handlers are registered at `0x1F9850` into a table at `0x3E96D0` [notes]. The earlier
notes matched names to PS2 numbers by handler order; the GC trace now confirms each
number directly.

Meanings in the table come from the PS2 handlers and script use [notes], confidence
**medium** unless marked. The initial research checked only GameCube numbering and
names. Subsequent [matching GameCube reconstruction](../../Script.md) establishes the
behaviour of 28 handlers; meanings of the remaining handlers are still **unverified**
on GameCube.

| Op | Name (GR8E69 symbol) | Meaning | PS2 handler | GR8E69 handler |
|---|---|---|---|---|
| `00` | BNE | jump by the argument (in words, from this instruction) if the top value is 0; pops it | `0x1F9E88` | `0x800f5388` |
| `01` | JMP | jump by the argument | `0x1F9ED8` | `0x800f53d8` |
| `02`–`05` | ADD, SUB, DIV, MULT | add, subtract, divide, multiply (in that order: `04` is divide) | from `0x1F9EF8` | `0x800f53f4`–`0x800f57b4` |
| `06` | NEG | negate | `0x1FA328` | `0x800f58f4` |
| `07` / `1E` | EQ / NE | equal / not equal | `0x1FA888` / `0x1FA998` | `0x800f5f44` / `0x800f607c` |
| `08`–`0B` | LT, GT, LTE, GTE | comparisons | — | `0x800f5cc0`, `0x800f5a3c`, `0x800f5dfc`, `0x800f5b78` |
| `0C`–`0E` | AND, OR, NOT | logical operations | — | `0x800f5970`, `0x800f59c0`, `0x800f5a10` |
| `0F` / `10` | LW / SW | push / store frame slot N (arguments and locals) | `0x1FABB8` / `0x1FAB70` | `0x800f62ec` / `0x800f62b0` |
| `11` | PUSH | push the following word | `0x1FAB40` | `0x800f6280` |
| `12` | POP | drop N values | `0x1FAB10` | `0x800f6254` |
| `13` | CAST | convert the top value between integer and float | `0x1FAAA8` | `0x800f61c4` |
| `14` | FNCALL | call a script function with N arguments at the code position in the following word; saves the return position and frame | `0x1FADE0` | `0x800f64e0` |
| `15` | RETURN | return, with the top value if the first sub-field is set | `0x1FAEC0` | `0x800f65b0` |
| `16` | BREAK | end the current handler (sets the engine's done flag) | `0x1FAF20` | `0x800f6614` |
| `17` | NEXTSTATE | go to the state on top of the stack, leaving and entering states along the state tree | `0x1FAF60` | `0x800f6650` |
| `18` / `19` | LG / SG | push / store the object's variable N ("global" in the GR8E69 names) | `0x1FAC48` / `0x1FAC00` | `0x800f6368` / `0x800f632c` |
| `1A` | BIFNCALL | call built-in function N (index into `bsbifunc.dat`) | `0x1FAE58` | `0x800f6544` |
| `1B` / `1C` | LM / SM | get / set field N of the object reference on the stack ("member") | `0x1FACD8` / `0x1FAC90` | `0x800f63e0` / `0x800f63a8` |
| `1D` | GP | push the engine-side object of this object, or of a local. Not used in the examined levels, so the meaning comes from the handler alone (confidence **low**) | `0x1FAD60` | `0x800f645c` |
| `1F` | TRACE | does nothing (a debugging marker) | `0x1FAF48` | `0x800f663c` |
| `20` | LS | push the class's shared value N | `0x1FAD10` | `0x800f6414` |

"—" means the PS2 address was not recorded in the earlier notes.

**Code positions** (FNCALL's extra word, and the `.sin` handler tables) [notes],
confidence **medium**: `(class depth << 24) | word offset`. Depth 0 is the root class's
code; for example `NativeBound` 0 → `Child` 1 → `NPC` 2.

**PS2 engine variables** [notes], confidence **medium**; GameCube equivalents
**unverified**:

| Address | Holds |
|---|---|
| `0x3E975C` | instruction pointer |
| `0x3E96C8` | stack pointer |
| `0x3E9760` | frame (arguments and locals) |
| `0x3E976C` | the current object's variables |
| `0x3E9768` | the code start of each class level |

## Built-in functions

**`bsbifunc.dat`** [notes] [re-check], confidence **high** for the layout. The header is
`BIF\0`, then the version (30) and, at `0x0C`, the count (**583**). From `0x14` come
16-byte entries, each holding:

- a word (always 0 in the entry checked; role unknown);
- the **upper-case CRC-32 of the function name**;
- the 16-bit function number;
- two 16-bit argument fields *a* and *b*;
- 16 bits of padding.

**Argument fields** [notes], confidence **medium**: *b* is the argument count, and
*b* − *a* = 1 when the function returns a value. *a* = `0xFFFF` (−1) means no arguments
and a returned value. Example: `GetFlexProperty` is entry 433 with *a* = 0 and *b* = 1,
so it takes one argument (the setting CRC) and returns its value.

**Names** [re-check] [GC], confidence **high**: all 583 CRCs equal the CRC-32 of the
**upper-cased** name of a GR8E69 `BIFunc_<name>` symbol, and none match with exact
case. The names are therefore already public in this repository's symbols and are not
repeated here. Before the GameCube symbols were available, the earlier PS2 notes had
guessed 28 roles from script use. Most were right, but some were wrong (for example
`0xB6` is `InitToInvalid`, and `0x9D` is `MatchGroup`). That is a caution for any role
inferred only from use.

**The engine's own table:**

- **PS2** [notes] [re-check]: at address `0x35D028`, 583 pairs of (CRC, code address) in
  `bsbifunc.dat` order. Entry 433 holds the `GetFlexProperty` CRC, which was re-checked.
  At start-up (`0x1FDD58`) the engine matches the file's CRCs against this table and
  builds a 16-byte-per-function run-time table [notes].
- **GameCube** [GC], confidence **high**: GR8E69 has the same table in `.data` at
  `0x802c3528`. It holds 583 pairs of (big-endian CRC, function address), with **the
  same CRCs in the same order** as the PS2 `bsbifunc.dat`, as reported by the contributor.
  A maintainer cross-check (2026-10-04, Codex-assisted) confirms that all **583** GameCube
  targets have matching `BIFunc_*` function symbols and upper-case name CRCs. The entry
  originally left unidentified is index 409 (zero-based), CRC `0xdb912483`, targeting
  `BIFunc_RegisterAlarm__FPPiPv` at `0x800d7a94`. That address also has a
  `gcc2_compiled.` marker; the marker does not replace the function symbol.
  `BSInitBIFuncs` (`0x800f8dac`) and `BSBifuncInvalid`
  (`0x800f9604`) exist. Their roles as the start-up matcher and the fallback for
  unmatched CRCs are plausible but **unverified**.

## Open questions

- The exact semantics of the remaining GameCube instructions. Subsequent
  [matching GameCube reconstruction](../../Script.md) establishes 28 handlers,
  including stack operations, branches, calls, return, all four binary arithmetic
  operations, negation, casts and all six numeric comparisons. Binary numeric type
  selectors encode right then left operand types; mixed operations round the integer
  operand to single precision. `BREAK` also advances by the signed argument;
  `TRACE` advances one word.
- GP (`1D`), which is unused in the examined levels.
- The first word of each `bsbifunc.dat` entry.
- The GameCube `.cbs` and `bsbifunc.dat` files, which were not examined, including
  their byte order.
