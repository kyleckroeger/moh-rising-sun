# Network-library reconstruction evidence

Eleven library fragments restore **16,448 code bytes in 32 functions**, plus
**2,080 initialized data bytes** and **1,434 BSS bytes**. Only the manifest-listed
retained functions are verified. This is a compatible subset of public library
reconstructions, not a recovered complete network stack or an identification
of the game's exact SDK release. Integration used AI assistance.

## References and attribution

The IP helper units and socket-library headers come from
[Cuyler36/dolsocketlibrary](https://github.com/Cuyler36/dolsocketlibrary/tree/e02356692695e64429dd70dc421a4a71a493e9c8),
commit `e02356692695e64429dd70dc421a4a71a493e9c8`.

The RSA Data Security, Inc. MD5 Message-Digest Algorithm implementation comes
from [Cuyler36/dolnetbp](https://github.com/Cuyler36/dolnetbp/blob/ba3846a83bf86505fe60e07aefe89e3e0ffc0e34/build/src/eth/md5.c),
commit `ba3846a83bf86505fe60e07aefe89e3e0ffc0e34`. Its complete upstream copyright
and permission notice remains in the source. The initial selected network headers and
six source files are unmodified reference files. Additional adaptations are
described below; individual upstream hashes and paths are recorded in
[dolphin-network-source.json](../config/GR8E69/dolphin-network-source.json).
The two Cuyler36 reference repositories supply no repository-wide license; no broader grant
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
| `IPRoute` | `0x8024b6c0` | 7 route/broadcast operations | 4,452 | 172 initialized bytes, 1,300 BSS bytes |
| `IPDnsDiagnostic` | `0x80250228` | 3 DNS name/resource operations | 1,740 | 536 initialized bytes |
| `DHCPDump` | `0x802508f4` | 1 DHCP diagnostic operation | 952 | 888 initialized bytes |
| `ethsec` | `0x802762e8` | 4 packet/checksum operations | 2,692 | 6 initialized bytes, 114 BSS bytes |
| `base64` | `0x80277bd8` | 2 Base64 operations | 780 | 66 bytes `.rodata` |

Every allocated generated section is placed from original named storage,
instruction references or unique complete-data evidence and checked in full.
The time-wait control object's original local name, size, address and file
scope are checked independently of its zero-filled BSS contents. No complete
historical source boundaries are inferred merely from a global function name.

## Older routing, diagnostics and Ethernet packet handling

The additional batch restores 10,616 bytes in 17 complete functions. It does not
change the progress denominator or grant credit for data and original callees.

`IPRoute` uses the same pinned SocketLibrary reference, with its older broadcast
rules reconstructed from the target: only all-one host portions are accepted in
the subnet and link-local checks; the original class A/B/C network fallback is
retained; the later prefix-bit-count restriction is absent. The original branches,
comparisons, masks and every generated instruction match. The complete route
unit's initialized data and three named BSS objects plus the routing table are
checked, including sizes and file scope. This is a historical behavior match,
not a change to modern networking behavior.

`DHCPDump` contains the diagnostic function and its two reference constant tables.
`IPDnsDiagnostic` contains the retained name/resource routines, their helpers and
constant-producing packet diagnostic code. The original `TypeStrings` symbol is
68 bytes: 17 pointers, not the reference's later 43. The two later record-type
cases (33 and 35) and their string helper are absent. The bounds check and jump
table confirm the older range. All emitted strings, tables, relocations and
complete retained text match; unused helper functions are explicitly enumerated
and stripped. These filenames identify project fragments of `IPDhcp.c` and
`IPDns.c`, not newly discovered historical source files. Other DNS/DHCP behavior
and state remain original context.

`ethsec`, `base64` and the four headers under `include/dolphin-ethernet/` derive
from [bfbbdecomp/bfbb](https://github.com/bfbbdecomp/bfbb/tree/8fb1c232addcacf44932c10c4cede41b43f98f40),
commit `8fb1c232addcacf44932c10c4cede41b43f98f40`. The reference CC0 notice is
preserved as `LICENSE.bfbb`. The MD5 header retains its separate RSA Data
Security, Inc. MD5 Message-Digest Algorithm copyright and permission notice.
The Ethernet headers are scoped to these units; they do not replace the socket
library's wider, partly unverified structures.

Integration adds the missing type include for Base64, explicit Base64 prototypes
and character-pointer casts in `ethsec`, and uses the existing `os.h` umbrella
for `OSTime`. The device-code access is a volatile 16-bit C read at the observed
`0x800030e6` address. The original filter exempts source port 53, but lacks the
reference's two port-1900 exemptions; removing those later branches reproduces
the target. Both checksum functions, the complete packet filter and post-send
function match, including all original external call targets. The private
`Ri` and `SendBuf` objects match their original names, sizes, file scope and
addresses. No synthetic packet behavior, instruction patch or assembly body
was introduced.

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

The wider socket/ethernet survey still exposes genuine version differences in TCP,
UDP, PPP and driver behavior and storage. The full stack and Ethernet driver are not accepted.
Matching helper groups do not justify substituting later structures or silently
accepting near-matching routines. The private original functions and unmatched
globals remain binary context.

Run the full source build, validation suite and snapshot capture as described
in [Progress.md](Progress.md). Original-object baseline verification remains
separate; neither this library match nor the complete analysis-image comparison
establishes emulator execution or working multiplayer/network behavior.
