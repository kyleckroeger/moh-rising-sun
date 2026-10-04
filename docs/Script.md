# Behaviour-script reconstruction

[kyleckroeger's PS2 research](research/README.md) supplied the opcode map,
class-description shapes and music-interface leads for this work. The accepted
bodies were independently reconstructed from the pinned GameCube GR8E69 executable
with Codex assistance. No PS2 source, offsets or file layout were assumed to apply
without checking the GameCube instructions and symbols.

## Accepted scope

Seven fragments in `src/script/` reconstruct **20 functions and 1,560 executable
bytes**: 16 interpreter handlers (968 bytes), event lookup (220 bytes), and three
music built-ins (372 bytes). Their exact symbols, ranges and dependencies are in
`config/GR8E69/script_*.json`. Original file markers support the `bsmachin.cpp`,
`bsfile.cpp` and `bsbifunc.cpp` groupings; these fragments are not complete original
translation units. Private interpreter globals retain the `bsmachin.cpp` file-record
scope in their manifests and remain original storage, with no source/data credit.

## Verified instruction behaviour

The interpreter stack uses four-byte slots and its top pointer addresses the current
value. Instruction arguments are the signed high 16 bits of a four-byte word. Byte
accesses below refer to GameCube big-endian memory, not PS2 file offsets.

| Handler | Behaviour established by the matched body |
| --- | --- |
| `BNE` | Pops one value; a nonzero value advances one instruction, while zero jumps by the signed argument in words relative to the current instruction. The original name is preserved. |
| `JMP` | Jumps by that same signed relative argument. |
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

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` matches every
allocated byte in these fragments. This is a working compiler profile, not proof
of the original release. No instructions are patched, no assembly bodies are
substituted, and no functions or sections are discarded or clipped. These fragments
generate only code; all external data remains uncredited original context.

The complete rebuilt 2,860,576-byte analysis image also matches the original,
including its header, allocated ELF bytes, entry point and BSS extent. Follow the
[build, test and snapshot process](Progress.md) for future changes. Runtime and
emulator behaviour remain untested. Arithmetic, comparison and remaining handlers,
class loading, message dispatch and music internals remain future work. The
[script](research/ps2/script-opcodes.md), [class-description](research/ps2/sin-class-descriptions.md)
and [music](research/ps2/pathfinder-music.md) research retains broader hypotheses and
cross-platform questions beyond the matched bodies here.
