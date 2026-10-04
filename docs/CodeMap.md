# Executable code map

The [decomp.dev treemap](https://decomp.dev/lifewillbeokay/moh-rising-sun)
includes the unfinished executable as grey boxes. Click a box to inspect its
function ranges. This inventory helps choose work; it is not reconstructed source
or an accepted linker split. Existing matched source units remain separate green
boxes, so a grey file group contains only that file's still-unmatched ranges.

At the initial 10.000995% snapshot, the map replaced the single remaining-code placeholder with **725 units**:

| Group | Grey boxes | Function-symbol records | Executable bytes |
| --- | ---: | ---: | ---: |
| Named file groups | 583 | 6,752 | 1,795,248 |
| Functions with unknown file ownership | 138 | 2,062 | 433,536 |
| Shared entry-point groups | 2 | 36 | 152 |
| Unidentified code or padding | 2 | — | 13,868 |
| Total unfinished | 725 | 8,850 | 2,242,804 |

Together with the 209 accepted source units/fragments (249,228 bytes), this covers
all 2,492,032 executable bytes exactly once. Matching progress at that snapshot was 10.000995%.
The original's 9,883 function-symbol records are accounted for by 1,033 accepted
records and 8,850 unfinished records. These are symbol counts, not a claim that
there are 9,883 independent function bodies.

The current verified snapshot has **330 accepted units/fragments (345,784 bytes)**
and **716 grey groups (2,146,248 bytes)**. Its 1,331 accepted and 8,552 remaining
function-symbol records still account for all 9,883 records. Grey groups can shrink, split or
disappear as source is accepted. See the [reuse index](ReuseMap.md) for ways to turn
this coverage map into productive work, and the [dependency map](Dependencies.md)
for shared helpers ranked by their unfinished callers.

## Evidence and uncertainty

`tools/code_map.py` reads the hash-pinned original through the existing ELF
reader. It uses only allocated executable sections, file records, compiler
markers and sized function symbols. Original bytes, data symbols, complete debug
information and historical workstation paths are not published. The selected
inventory is stored in the public verification snapshot's `code_map` field.

- **`Unmatched files/… [local symbols only]`** groups local function symbols under
  their preceding `STT_FILE` record. It makes no claim about nearby globals.
- **`Unmatched files/… [inferred file group]`** also includes globals that fall
  wholly between `gcc2_compiled.` markers in two immediately adjacent file records.
  Local functions must agree with the interval; conflicting local evidence or
  overlapping marker intervals disables inference. The snapshot preserves the
  marker interval and each function's evidence type. Missing-marker files are
  never skipped over, and the last marker is never extended to the section end.
- **`Unknown file/…`** groups consecutive function ranges by address when there is
  insufficient file evidence. Its boundaries are for navigation, not a proposed
  original translation unit. Do not assign ownership from a function's spelling
  alone or attach all global symbols to the final file record.
- **`Shared entry points/…`** preserves the two overlapping `_savegpr_14`–`31` and
  `_restgpr_14`–`31` symbol families. Each family occupies 76 distinct bytes. All 18
  names, addresses and declared sizes remain in the snapshot, while the treemap
  uses one union-sized function-range item per family to avoid double counting.
- **`Unidentified code or padding/…`** covers four gaps across `.init` and `.text`
  that have no sized function symbol. They remain executable-denominator bytes,
  not invented functions or automatically classified padding. Their address
  ranges are in the snapshot and the report's section items.

Repeated file basenames have a file-record ordinal to keep their groups distinct.
No unfinished group receives a fabricated `source_path`, source credit, inferred
prototype, class layout or claim of a complete original source-file boundary.

## Starting points

Examples of remaining file groups include `player.cpp` (142 function ranges,
62,976 bytes), `collision.cpp` (15 ranges, 11,832 bytes), `BotWeapon.cpp` (three
ranges, 776 bytes) and `MathFun.cpp` (ten ranges, 1,268 bytes). These are candidates
for investigation, not estimates of difficulty or promises of independent builds.
The file attribution of their global functions is marked as inferred.

Coordinate a unit or function in an issue before beginning. Treat the map as an
index into the pinned executable and review the actual instructions and symbol
evidence before creating a source manifest. New matching credit still requires
the full-image verification described in [Progress.md](Progress.md).

## Updating and validating the map

`python3 tools/progress_report.py --capture` regenerates the map from the pinned
original after a fresh source build. It removes accepted source coverage, unions
overlapping function ranges, and retains all remaining bytes. The public exporter
checks accepted plus unfinished ranges against the pinned section addresses and
sizes, rejecting missing bytes, overlaps, out-of-section ranges and invalid shared
entry-point evidence. CI can perform these checks without the original game.

Mapping changes alone must not change the matched-byte numerator or executable
denominator. The tests include missing file markers, conflicting ownership,
out-of-order marker intervals, shared entry points, repeated file names, corrupted
coverage and source/map overlaps. Original-dependent tests and a full source
rebuild remain part of local verification; public CI validates the captured map.
