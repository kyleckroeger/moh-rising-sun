# Network-library reconstruction evidence

Six library fragments restore **5,832 code bytes in 15 functions**, plus
**412 initialized data bytes** and **20 BSS bytes**. Only the manifest-listed
retained functions are verified. This is a compatible subset of public library
reconstructions, not a recovered complete network stack or an identification
of the game's exact SDK release. Integration used AI assistance.

## References and attribution

Five IP helper units and the additional network headers come from
[Cuyler36/dolsocketlibrary](https://github.com/Cuyler36/dolsocketlibrary/tree/e02356692695e64429dd70dc421a4a71a493e9c8),
commit `e02356692695e64429dd70dc421a4a71a493e9c8`.

The RSA Data Security, Inc. MD5 Message-Digest Algorithm implementation comes
from [Cuyler36/dolnetbp](https://github.com/Cuyler36/dolnetbp/blob/ba3846a83bf86505fe60e07aefe89e3e0ffc0e34/build/src/eth/md5.c),
commit `ba3846a83bf86505fe60e07aefe89e3e0ffc0e34`. Its complete upstream copyright
and permission notice remains in the source. The selected network headers and
six source files are unmodified reference files; their individual upstream
hashes and paths are recorded in
[dolphin-network-source.json](../config/GR8E69/dolphin-network-source.json).
The reference repositories supply no repository-wide license; no broader grant
is asserted. See [NETWORK_NOTICE](../src/dolphin/NETWORK_NOTICE).

## Retained coverage

| Unit | Text start | Retained functions | Code bytes | Other generated storage |
| --- | --- | ---: | ---: | --- |
| `IFFifo` | `0x8024d70c` | 1 (`IFDump`) | 268 | 30 bytes `.sdata` |
| `IFRing` | `0x8024d818` | 4 ring-buffer operations | 444 | None |
| `IPLcp` | `0x8024a7f4` | 1 (`PPPDumpLCP`) | 336 | 308 bytes `.data`, 10 bytes `.sdata2` |
| `IPOpt` | `0x80251070` | 2 route-option operations | 716 | None |
| `IPTcpTimeWait` | `0x802500b0` | 1 (`TCPLookupTimeWaitInfo`) | 376 | 20 bytes `.bss` |
| `md5` | `0x80276d6c` | 6 digest/encoding functions | 3,692 | 64 bytes `.data` |

Every allocated generated section is placed from original named storage,
instruction references or unique complete-data evidence and checked in full.
The time-wait control object's original local name, size, address and file
scope are checked independently of its zero-filled BSS contents. No complete
historical source boundaries are inferred merely from a global function name.

## Compiler, stripping and limits

The working profile is CodeWarrior `GC/1.2.5n`, `-O4,p -inline auto`, signed
`char`, and the existing release SDK settings. The network headers precede the
existing Dolphin SDK headers. SN's native linker removes reference functions
absent from the target; all their names are enumerated and checked. Inlined
helper code is included in its retained caller's complete byte comparison.

`IPLcp` includes unresolved declarations used solely by discarded functions.
The existing relocation audit proves that no retained code or data references
those declarations. Every emitted constant remains accounted for and is
compared even when associated reference functions are stripped. Data and BSS
contribute no code credit.

The wider socket/ethernet survey exposed genuine version differences in TCP,
UDP, PPP and driver behavior and storage. The full stack is not accepted.
Matching helper groups do not justify substituting later structures or silently
accepting near-matching routines. The private original functions and unmatched
globals remain binary context.

Run the full source build, validation suite and snapshot capture as described
in [Progress.md](Progress.md). Original-object baseline verification remains
separate; neither this library match nor the complete analysis-image comparison
establishes emulator execution or working multiplayer/network behavior.
