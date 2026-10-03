# sys_memory.cpp: allocation operators

Four game allocation operators are reconstructed in `src/sys_memory.cpp`:
**192 code bytes and 32 read-only data bytes**. Only code earns progress credit.
AI assistance was used for reconstruction and verification.

| C++ operation | Original symbol | Address | Code bytes |
| --- | --- | --- | ---: |
| `operator new` | `__builtin_new` | `0x801a8910` | 64 |
| `operator new[]` | `__builtin_vec_new` | `0x801a8950` | 64 |
| `operator delete` | `__builtin_delete` | `0x801a8990` | 32 |
| `operator delete[]` | `__builtin_vec_delete` | `0x801a89b0` | 32 |

The `sys_memory.cpp` file record and its local compiler marker precede the
`configfile.cpp` marker at `0x801a89d0`. These global functions fall within that
adjacent-marker interval; file association is therefore inferred. Their names,
addresses and sizes are direct symbol evidence. This is a fragment of the original
unit; the allocator implementation remains original context.

## Behavior and dependencies

Both allocation operators read the four-byte global `g_memLabel` at `0x802c7dc0`.
If it is null, they use their respective strings, `operator new` and
`operator new[]`. The original instructions establish both string addresses;
the complete compiled `.rodata` block, including alignment padding, matches
`[0x802b77e8, 0x802b7808)`. The strings are neither guessed labels nor arbitrary
copies selected elsewhere in the executable.

They call `DWI_alloc` at `0x801a8670` with the selected label, requested size and
the value **1024**. The third argument is named `flags` descriptively; its complete
semantics are not recovered, and it is not claimed to mean alignment. `DWI_alloc`
is declared with two integer parameters, consistent with its calls to the local
`allocSmall__FPCcii`. Passing the unsigned allocation size preserves the target
compiler's observed 32-bit argument bits; this does not establish portable
behavior for every size.

Both deletion operators call `DWI_free` at `0x801a8894` and ignore its result.
That routine's observed paths return an integer status; its implementation and
`MEM_free` remain unreconstructed. The wrappers add no null check, exception,
new-handler loop, or replacement failure behavior. `g_memLabel` remains externally
owned data. Its declaration as a pointer to const characters reflects its use;
the original typedef and const spelling are unknown.

The [dependency scan](Dependencies.md) finds **424 distinct unfinished caller
ranges** across these four operators. Many container instantiations use this
path. Reconstructing these small wrappers establishes a useful allocation
boundary; it does not reconstruct their callers or the underlying heap.

## Verification

ProDG 3.8.1 with `-O2 -G0` compiles ordinary C++ operator definitions to the
original SN/GCC symbol names. The manifest checks all four function ranges,
bindings, external definitions and both generated sections. All 192 code bytes
and 32 data bytes match. No code/data is discarded or patched.

The complete analysis-image comparison includes this fragment. The original
compiler release is still unconfirmed, and runtime/emulator behavior has not
been tested. Use `python3 tools/reconstruct.py` and the validation suite for
extensions; follow [Progress.md](Progress.md) before updating coverage.
