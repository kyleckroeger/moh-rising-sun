# Network-library reconstruction evidence

Sixteen library fragments restore **32,296 code bytes in 86 functions**, plus
**2,542 initialized data bytes** and **15,874 BSS bytes**. Only the manifest-listed
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
| `IPCore` | `0x8024a944` | 6 lookup/connect/output operations | 1,760 | 14 initialized bytes |
| `UDPTransport` | `0x8024c824` | 13 UDP operations and callbacks | 3,680 | 2 initialized bytes, 8 BSS bytes |
| `TCPTransport` | `0x8024d9d4` | 8 TCP packet/ring operations | 1,664 | None |
| `TCPUser` | `0x8024ebd4` | 19 TCP API operations and callbacks | 5,340 | 90 initialized bytes, 8 BSS bytes |
| `SocketAPI` | `0x80249aa8` | 8 socket operations and callbacks | 3,404 | 356 initialized bytes, 14,424 BSS bytes |

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

## Older transport ABI and socket operations

The next batch restores 15,848 code bytes in 54 whole functions from the same
pinned SocketLibrary reference. These five project filenames identify reviewed
fragments of `IP.c`, `IPUdp.c`, `IPTcp.c`, `IPTcpUser.c` and `IPSocket.c`;
they do not assert recovered historical translation-unit boundaries. All emitted
storage, including jump tables and diagnostic strings, is checked in full.

Two scoped headers under `include/dolphin-network-2003/` preserve the older ABI.
They precede the unchanged reference headers only for these five units. The
layout evidence comes from independent original callers and their exact generated
instructions, not the reference's later offsets:

| Record | Observed evidence |
| --- | --- |
| `IPInfo` | 32-bit polling count at `0x04`; local/remote sockets at `0x08`/`0x10`, queue link at `0x18`; total 32 bytes. Later multicast flag fields are absent from this declaration. |
| `IFDatagram` | Type at `0x10`, route destination at `0x18`, callback/parameter at `0x28`/`0x2c`, vector count at `0x30`, vectors at `0x34`; 60-byte base size independently reproduced by UDP and TCP allocation arithmetic. Bytes `0x12..0x17` and `0x1c..0x27` remain explicitly unresolved. |
| `IPInterface` | Output, cancel, allocation and free function pointers at `0x60`, `0x64`, `0x68`, `0x6c`. This is a prefix declaration; no full interface size or trailing-field layout is asserted. |
| `TCPInfo` | State/flags at `0x8c`/`0x90`, send callback at `0x180`, receive callback at `0x1b4`, urgent callback at `0x1d0`, retransmission alarm at `0x1f8`, linger alarm at `0x280`. Socket cleanup independently reads `node` at `0x2c8` and frees 720 bytes. The `0x2a8..0x2c7` interval remains unresolved. |

The later SACK scoreboard, acknowledged-receive-sequence field and delayed-ACK
alarm are absent from this older TCP declaration. Matching the retained uses
does not establish meanings for unused fields or validate unrelated APIs from
the reference headers. Unknown intervals are byte storage with no invented
semantic field names. These declarations and the verified call signatures are
reusable evidence for future transport work.

`IPCore` restores five-argument lookup without later multicast membership rules,
the observed anonymous-port range 1024 through 5000, and TCP-only duplicate/time-
wait checks. Packet output always consults the route table, rejects packets over
the interface MTU, and computes the original UDP/TCP checksums. Its checksum
helper is compiled inline; the discarded out-of-line copy earns no extra credit.

`UDPTransport` restores the original datagram setup, TTL selection, cancellation
callback order and notification destination check. The target's initial port is
1024. The older send-length bound and absence of the later multicast-specific
branches are preserved as historical behavior, not proposed networking changes.

`TCPTransport` restores the older response signature without a TCP control-block
argument, response packet construction, receive helpers without the later urgent-
status output parameter, and source-quench congestion window behavior.
`TCPUser` restores sequential abort callbacks, older close/cancel/reset behavior,
receive completion when any bytes are available, and nonblocking send results.
Its reset function includes the compiler-inlined cancellation logic. Every
retained call target and generated control-flow instruction matches.

`SocketAPI` restores asynchronous-connect result handling and the older placement
of nonblocking would-block handling in the socket wrapper. `SocketTable`, each
retained small-BSS object, their original names, file-local bindings, sizes and
addresses are verified. The later UDP buffer configuration globals are absent.
The original resolver object is outside this fragment and earns no source credit.
The original panic filename and line are `IPSocket.c:494`; the stripped address-
formatting helper preserves the original shared format string. No copied original
code, relocation masking or instruction patch is used.

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

The wider socket/ethernet survey still exposes genuine version differences in
remaining TCP, UDP, PPP and driver behavior and storage. The full stack and Ethernet driver are not accepted.
Matching helper groups do not justify substituting later structures or silently
accepting near-matching routines. The private original functions and unmatched
globals remain binary context.

Run the full source build, validation suite and snapshot capture as described
in [Progress.md](Progress.md). Original-object baseline verification remains
separate; neither this library match nor the complete analysis-image comparison
establishes emulator execution or working multiplayer/network behavior.
