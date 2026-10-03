# Progress reports and decomp.dev

The project exports the [objdiff report v2 schema](https://github.com/encounter/objdiff/blob/0c48d711c7bd51f791b353d7d85ba948b277e2f2/objdiff-core/protos/report.proto)
using its own byte-verification tooling. GitHub Actions uploads `report.json` in
an artifact named `GR8E69_report`, as required by the
[decomp.dev integration guide](https://decomp.wiki/tools/decomp-dev).
There is no decomp.dev-specific repository metadata file.

## Verification boundary

The public workflow validates a **locally verified snapshot**. It does not have
the original executables and does not rerun the proprietary compiler toolchain.
`progress/GR8E69.snapshot.json` records hashes of every source file, header,
configuration file and build script, plus verified unit counts and object hashes.
Snapshot schema 2 also includes a selected executable-symbol map for unfinished
code. It is generated from the pinned original during capture.
Changes, additions and removals in those directories make CI fail until someone
with the pinned original completes verification and refreshes the snapshot.
Documentation-only changes do not require rebuilding.

To refresh after an accepted source/tooling change, on a supported local machine:

```sh
python3 tools/reconstruct.py
python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
```

Capture rechecks the complete rebuilt image against its pinned identity, original
load layout, source ranges and compiled unit hashes/bytes. It emits only selected
public metadata, never original bytes or local filesystem paths. Commit the
snapshot with the source change. Maintainers must review snapshot updates;
fingerprints detect stale inputs, not fabricated assertions or a malicious verifier.

CI runs the tests, checks the complete input inventory and generates the objdiff
report. Anyone can reproduce that public export without a game image:

```sh
python3 tools/progress_report.py
```

## Counting rules

The overall denominator is every allocated executable byte in the pinned
`MOH3RDVD.ELF`: 2,492,032 bytes. Only nonoverlapping, verified source-built function
bytes earn credit. Restored library code and reconstructed game code are distinct
categories. Accepted units may be fragments of an original translation unit.
Their categories describe accepted ranges, so 100% within an accepted category
does not mean all game code or all library code is done.

Unreconstructed code is represented by named file groups, explicitly unassigned
function groups, shared entry-point groups, and unidentified code/padding ranges.
See [the code map](CodeMap.md) for evidence and limitations. All are generated
inventory units with zero matching credit. Their ranges plus accepted source
must partition the complete executable with no gaps or overlaps. Inferred file
groups are labeled; their game vs. library ownership is not exhaustively
classified. Data, BSS and original context earn no code credit. Unknown total
function/unit/data counts are omitted rather than inferred from overlapping
symbol entries, synthetic navigation groups or partial source boundaries.
No fuzzy partial-match credit is used. This is progress on the initial Disc 1
executable, not on both discs or the whole game, and runtime remains untested.

## Registering the project

The project is registered at
<https://decomp.dev/lifewillbeokay/moh-rising-sun>.

Once a successful default-branch workflow has uploaded its report, a repository
admin can add the repository at <https://decomp.dev/manage/new>, naming the game
**Medal of Honor: Rising Sun** and platform **GameCube**. Repository creation and
report publication do not themselves register a project on decomp.dev. Its GitHub
app is optional for faster updates and PR comments; the site also polls reports.
