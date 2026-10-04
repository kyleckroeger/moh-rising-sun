# Behaviour-script reconstruction

[kyleckroeger's PS2 research](research/README.md) supplied the opcode map,
class-description shapes and music-interface leads for this work. The accepted
bodies were independently reconstructed from the pinned GameCube GR8E69 executable
with Codex assistance. No PS2 source, offsets or file layout were assumed to apply
without checking the GameCube instructions and symbols.

## Accepted scope

Eleven fragments in `src/script/` reconstruct **32 functions and 5,036 executable
bytes**: 28 interpreter handlers (4,444 bytes), event lookup (220 bytes), and three
music built-ins (372 bytes). Their exact symbols, ranges and dependencies are in
`config/GR8E69/script_*.json`. Original file markers support the `bsmachin.cpp`,
`bsfile.cpp` and `bsbifunc.cpp` groupings; these fragments are not complete original
translation units. Private interpreter globals retain the `bsmachin.cpp` file-record
scope in their manifests and remain original storage, with no source/data credit.

## Verified instruction behaviour

The interpreter stack uses four-byte slots and its top pointer addresses the current
value. The high 16 bits of a four-byte instruction word carry an argument or type
selector. Branch and slot arguments and the `CAST` selector are signed; arithmetic,
`NEG` and comparison selectors are unsigned. Byte accesses below refer to GameCube
big-endian memory, not PS2 file offsets.

| Handler | Behaviour established by the matched body |
| --- | --- |
| `BNE` | Pops one value; a nonzero value advances one instruction, while zero jumps by the signed argument in words relative to the current instruction. The original name is preserved. |
| `JMP` | Jumps by that same signed relative argument. |
| `ADD` / `SUB` / `DIV` / `MULT` | Applies the named operation to the lower (left) and top (right) operands, writes the result in the lower slot, pops one slot and advances one word. Integer pairs produce integers; mixed or float pairs produce float bits. Unsupported type pairs produce zero. |
| `NEG` | Negates an integer or float in place according to the type selector; other selectors replace the value with zero. Advances one word. |
| `GT` / `GTE` / `LT` / `LTE` / `EQ` / `NE` | Compares the lower (left) operand with the top (right) operand, stores integer zero or one in the lower slot, pops one slot and advances one word. Unsupported type pairs produce zero. |
| `CAST` | Selector 2 converts the top float to an integer; selector 3 converts the top integer to a float. Other selectors leave the value unchanged. Advances one word without popping. |
| `NOT` | Replaces the top value with normalized integer logical negation and advances one word. |
| `POP` | Subtracts the signed argument from the stack pointer and advances one word. |
| `PUSH` | Pushes the following word and advances two words. |
| `SW` / `LW` | Stores and pops, or pushes, a frame slot indexed by the signed argument. |
| `SG` / `LG` | Performs the corresponding access through the current globals pointer. |
| `SM` / `LM` | Treats the top value as an address of integer slots. Store takes the value below it and pops both; load replaces the address with the selected slot. No complete object layout is implied. |
| `FNCALL` | Saves the frame and return address in two stack slots. Instruction byte 1 gives the argument count. The following word selects a code-base entry with its high byte and a word offset with its low 24 bits. |
| `BIFNCALL` | Sets the current built-in index from the signed argument, calls that table entry with the stack-pointer address and current native object pointer, then advances one word. |
| `RETURN` | Uses instruction byte 0 to locate the saved frame and return address. Byte 1 selects whether to keep the top value at the old frame or discard the frame's values. |
| `BREAK` | Sets the stop flag to one and also advances the instruction pointer by the signed argument. |
| `TRACE` | Advances one word without other work. |

Valid stacks, indices, code positions and referenced storage are preconditions of
the original interpreter. The reconstruction preserves its accesses and ordering;
it adds no range checks or fixes for malformed scripts. Pointer/integer conversions
describe the original 32-bit target, not a host-portable interpreter implementation.

## Numeric representation and conversions

The matched numeric handlers establish selector 2 for a signed 32-bit integer and
selector 3 for a single-precision float. Stack slots retain their four-byte value
bits; interpreting a float slot is different from converting an integer to float.
The local unions in negation and casting expose the result's float representation
and integer storage bits for the target compiler, without asserting a larger runtime
type or a portable host implementation.

For binary arithmetic and the six comparisons, the high-halfword selector encodes
the **right operand's type first**, followed by the left operand's type:

| Selector | Lower / left slot | Top / right slot |
| --- | --- | --- |
| `0x0202` | integer | integer |
| `0x0203` | float | integer |
| `0x0302` | integer | float |
| `0x0303` | float | float |

Mixed arithmetic and comparisons convert the integer to single precision before
applying the operator. Float arithmetic uses single-precision addition, subtraction,
division or multiplication and stores the result bits in the integer stack slot.
Subtraction and division preserve lower-slot minus/divided-by top-slot order. The
integer division path uses signed division, truncating representable results toward
zero. No overflow handling or divide-by-zero guard is present in these handlers.

In mixed comparisons, large integers can therefore lose precision before equality
or ordering is decided. Float
comparisons use the original unordered comparison instructions: NaN makes `NE` true
and the other five comparisons false; positive and negative zero compare equal.
`NEG` uses the target's float sign-negation instruction for float values.

`CAST` uses the target's truncation-toward-zero instruction for float-to-integer
conversion. Integer-to-float conversion rounds to single precision before storing
the result bits. No clamps, error reporting or checks for invalid conversions have
been added. These are verified target-instruction observations, not a claim that
host C++ gives defined results for out-of-range float casts or signed negation
overflow.

The arithmetic fragment generates 60 read-only bytes: four zero literals, four
eight-byte integer-conversion constants and 12 alignment bytes. `ADD`'s zero is at
`0x802a5054`, immediately before its aligned conversion constant at `0x802a5058`.
The other three zero/conversion pairs begin at `0x802a5060`, `0x802a5070` and
`0x802a5080`. A descriptive one-element constant array in `.rodata.add_initial`
places the first four-byte literal separately from the 56-byte compiler pool. This
is a storage annotation, not a recovered historical identifier or a claim about
original source declarations. Every generated byte, including alignment, is checked;
no section or padding is dropped and no instruction is patched.

The comparisons add six eight-byte conversion constants at
`0x802a5090`–`0x802a50bf`, and casting adds one at `0x802a50c0`. All **116 generated
read-only bytes** are compared separately from executable progress.

## Event lookup and music interfaces

`BSFindAnyEventHandler` searches every class level and present state, returning one
on the first matching event number and zero otherwise. This independently confirms
the research's 28-byte level stride, state-pointer array at level `+8`, halfword
state count at `+0x0c`, event-list pointer at state `+4`, byte event count at state
`+0x10`, eight-byte event stride and halfword event number at event `+4`. The class
holds its level pointer at zero and halfword level count at `+0x58`.

The local views expose only those accesses. Unknown bytes remain opaque, and the
views do not establish complete state/class allocations, historical member names,
or GameCube `.sin` serialization. Do not allocate objects from these prefixes.

The shared `ScriptBuiltins.h` view describes the 16-byte runtime table stride seen
in built-in dispatch and music wrappers. The function pointer is at zero. The music
wrappers read signed halfwords at `+0x0c` to locate arguments and at `+0x0a` to adjust
the stack after the call. The latter field is re-read after the music call, preserving
the original dependency on current table state. Other fields remain unknown.

`PathfinderSetLevel`, `PathfinderEvent` and `PathfinderSetLatency` each forward their
first integer argument to the corresponding `MUSIC_*` routine. Those music routines,
the table's initialization, and `PathfinderFadeVolume` remain original context.
Historical return-type spelling for ignored-return interfaces is not established
by this reconstruction.

## Verification and next work

All eight accepted `bsmachin.cpp` fragments use ProDG **3.9.3** with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. This profile reproduces all 28
accepted interpreter handlers and their generated constants. Event lookup and music
built-ins retain their verified ProDG 3.8.1 profile.

The arithmetic work distinguishes these working profiles for the tested source:
ProDG 3.5, 3.5b140, 3.7 and 3.8.1 remove the initial float-temporary store from the
straightforward `ADD` candidate, producing 308 rather than 320 bytes. ProDG 3.9.3
preserves it and matches all four arithmetic bodies once the literal storage is
placed as described above. The previous 24 handlers also match with 3.9.3; changing
their compiler profile earns no new source credit. These comparisons establish a
working profile, not the historical compiler release or original source spelling.

No instructions are patched, no assembly bodies are substituted, and no functions
or sections are discarded or clipped. These fragments generate code and the 116
read-only constant bytes described above. Generated data
earns no executable credit, and all external storage remains original context.

The complete rebuilt 2,860,576-byte analysis image also matches the original,
including its header, allocated ELF bytes, entry point and BSS extent. Follow the
[build, test and snapshot process](Progress.md) for future changes. Runtime and
emulator behaviour remain untested. Five interpreter handlers remain unfinished:
`AND`, `OR`, `LS`, `GP` and `NEXTSTATE` (1,152 executable bytes). Class loading,
message dispatch and music internals also remain future work. The
[script](research/ps2/script-opcodes.md), [class-description](research/ps2/sin-class-descriptions.md)
and [music](research/ps2/pathfinder-music.md) research retains broader hypotheses and
cross-platform questions beyond the matched bodies here.
