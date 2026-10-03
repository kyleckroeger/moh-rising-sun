# Direct dependency map and next shared-code targets

Run `python3 tools/dependencies.py` to generate
`build/audit/dependencies.json` from the pinned original. The graph remains
Git-ignored. It contains symbol names, branch sites, target evidence, candidate
dependencies and rankings, with no source-progress credit. Accepted status comes
from the current manifests; this analysis command does not verify their builds.

The initial scan found **9,849 nonoverlapping function ranges**, **31,677 direct
dependency sites**, **105 unresolved direct sites** and **4,777 register-indirect
call sites**. A range may contain several overlapping function symbols; aliases
are preserved together instead of duplicating instructions or inventing unique
ownership. Direct dependency sites include recursive calls and candidate
inter-function branches, not just ordinary calls.

Rankings count distinct unfinished caller ranges, not call sites. Recursive
self-calls do not increase that count. Caller-byte totals are unique within each
row but overlap across rows; they are not a prediction of matching coverage.

## Useful next work packets

Counts below follow the STL helpers and allocation-operator additions in this
revision. They will change as manifests grow; regenerate the report for current
counts. A small function with many callers is a type/contract research lead,
not necessarily the easiest source match.

| Target | Address | Code bytes | Unfinished callers | Work to unlock |
| --- | --- | ---: | ---: | --- |
| `CMatrix::InitClass` | `0x8007a9b4` | 132 | 173 | Recover the matrix field layout and initialization values, then check assignment and multiplication against the same declaration. Do not assume a conventional 4x4 layout. |
| `DWI_alloc` | `0x801a8670` | 212 | 143 | Follow the small-allocation path and main heap call; establish flag and failure behavior. Allocation operators now provide a verified caller. |
| `CMatrix::operator=` | `0x8007aa38` | 176 | 127 | Recover member-copy semantics and metadata alongside initialization; preserve the original return convention. |
| `FEHashUpper` | `0x800499a4` | 100 | 116 | Establish exact character conversion, signedness and hash recurrence; find callers' string ownership without replacing the algorithm with a generic hash. |
| `DWI_free` | `0x801a8894` | 64 | 75 | Recover `freeSmall`'s result contract and the fallback to `MEM_free`. |
| `CMatrix::Multiply` | `0x802794b8` | 836 | 75 | Reuse the matrix declaration once independently supported; investigate aliasing and floating-point/compiler settings. |
| `GetStringCRC` | `0x801acb98` | 80 | 60 | Identify the CRC table/algorithm and signedness before selecting a public implementation. |

The first priority for shared game-type work is the **CMatrix family**. The
larger library path remains [STLport container instantiations](STLport.md), with
the pointer-vector data-ownership blocker documented there. These are independent
research tracks; no game layout needs to be invented to finish the vector work.

## Evidence and limitations

The scanner decodes aligned PowerPC I-form and B-form branches inside sized
executable function symbols. It handles signed displacements, absolute branches,
link bits and conditional branch options. A destination must equal an original
function entry to become a named dependency. Cross-function interior destinations
and unknown destinations remain unresolved. Ordinary intra-function control flow
is omitted. Unlinked inter-function branches are labeled `branch`, not proven
tail calls.

This is not control-flow reachability analysis. A syntactic branch can be on a
path never executed. Register-indirect calls are counted without inferring their
targets; virtual dispatch, callbacks and indirect tail branches are not resolved.
The report does not recover data references, vtables, C++ types, or dynamic call
frequencies. Source file ownership is not inferred from call relationships.

Synthetic tests cover backward/absolute/conditional branches, address wrap,
indirect calls, returns, aliases, repeated calls, recursive calls, unresolved
destinations, and invalid function bounds. They run without original game files.
The graph is a prioritization aid; source acceptance still requires the complete
verification described in [Progress.md](Progress.md).
