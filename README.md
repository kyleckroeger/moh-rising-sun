# Medal of Honor: Rising Sun

An early matching decompilation of **Medal of Honor: Rising Sun** for GameCube, targeting the USA release **GR8E69, Disc 1, revision 0**. The initial game-code target is the disc's `MOH3RDVD.ELF`.

[![Verified progress snapshot](https://github.com/lifewillbeokay/moh-rising-sun/actions/workflows/progress.yml/badge.svg)](https://github.com/lifewillbeokay/moh-rising-sun/actions/workflows/progress.yml)

**Matching source coverage is 12.813640%: 319,320 of 2,492,032 executable code bytes, across 1,260 functions.** This comprises 2,648 bytes of reconstructed game-specific C++, 51,640 bytes restored from Lua 4.0.1, 166,200 bytes restored from public Dolphin SDK reconstructions, 53,892 bytes restored from Newlib, 5,532 bytes restored from GCC runtime helpers, 33,576 bytes restored from STLport helpers and container instances, and 5,832 bytes restored from network-library helpers and the RSA Data Security, Inc. MD5 Message-Digest Algorithm. The complete rebuilt analysis image is verified; the remainder stays original binary context. AI-assisted contributions are welcome; see [CONTRIBUTING.md](CONTRIBUTING.md).

Progress uses all bytes in the original allocated executable sections (`.init` and `.text`) as its denominator. It counts verified, nonoverlapping source-built function bytes only. The 28,786 source-built data bytes and 114,884 source-owned BSS bytes, stripped library functions, original context and debug metadata earn no code-progress credit. See [Lua evidence and attribution](docs/Lua.md) and [MathFun evidence](docs/MathFun.md), and [Dolphin SDK evidence and attribution](docs/Dolphin.md), [Newlib evidence and attribution](docs/Newlib.md), [GCC runtime evidence](docs/Libgcc.md), [STLport evidence](docs/STLport.md), [allocation-operator evidence](docs/Memory.md), and [matrix layout evidence](docs/Matrix.md), and [network-library evidence](docs/Network.md).

## Verified starting point

- `MOH3RDVD.ELF`: 3,628,980 bytes; SHA-1 `dd797412be7d631e78c98f194a1bb47981174d75`.
- 15,507 symbol entries, including 9,883 function entries and 954 source-file entries. Entries are not necessarily unique functions or complete original source units.
- Preserved DWARF 1 information covers seven Nintendo GBA-library compilation units. It is not full game-wide type information.
- The original-object relink matches the complete 2,860,576-byte DOL derived from the ELF, including its header. Every original allocated ELF file byte, entry point, and BSS extent is checked separately.
- The source build passes that same complete-image comparison with six MathFun functions, four allocation operators, 16 CMatrix functions, 19 Lua units, 89 Dolphin SDK units plus six network-library fragments, 99 Newlib units/fragments, eight GCC runtime variants and 92 STLport helpers/container instances compiled from source. Code and generated data are both checked; coverage is reported by category.

This comparison is against a derived analysis image. It does **not** mean the original ELF's debug/symbol tables have been reproduced, the on-disc boot DOL has been replaced, or the game has been tested in an emulator. The boot DOL is a separate 206,016-byte program containing references to the game ELFs.

## Join the project — AI-assisted contributions welcome

Want to help bring Rising Sun back to readable source? Human and AI-assisted
contributions are welcome: reconstruct game functions, investigate compiler
behavior, improve verification tools, document evidence, or test the loading path.
You do not need to use AI to contribute. AI-generated suggestions receive the same
review and byte-verification requirements as other changes.

Start with the [next work](#next-work) below and [contribution guide](CONTRIBUTING.md).
Open an issue to coordinate a source unit or investigation, then send a focused
pull request with your evidence, verification results and any AI assistance used.
Pseudonymous contributors are welcome. Preserve upstream credit and license notices.

Bring your own game copy for local verification. This repository contains source,
configuration and tooling; please do not upload game images, executable binaries,
assets, compiler binaries or complete debug dumps to commits, issues or PRs.

## Progress integration

Explore the [live decomp.dev map](https://decomp.dev/lifewillbeokay/moh-rising-sun)
to find unfinished functions. The remaining code is divided into 734 grey boxes:
named file groups where supported by symbols, unknown-file groups, shared entry
points and unidentified bytes. Inferred ownership is labeled and earns no matching
credit. See [code-map evidence and starting points](docs/CodeMap.md) and the
[reuse index and ranked next work](docs/ReuseMap.md). The new
[direct dependency map](docs/Dependencies.md) ranks shared helpers by unfinished
callers and preserves unresolved indirect calls.

The [progress workflow](.github/workflows/progress.yml) publishes an objdiff v2
`GR8E69_report` artifact compatible with decomp.dev. CI validates source hashes
against a locally verified snapshot; it does not rebuild the game. Source changes
require a fresh local complete-image verification before their progress can be
published. See [progress reporting and registration](docs/Progress.md).

## Local setup

Requires Python 3.9+ and macOS ARM64. Source compilation also uses existing Rosetta support. Tools are downloaded into ignored `build/tools/` and `build/compiler/` with pinned SHA-256 checksums. ProDG 3.8.1 with `-O2 -G0` is the working MathFun profile; the restored Lua library adds `-ffast-math` and uses native unused-function stripping. The SDK units use CodeWarrior GC/1.2.5n; EXI uses `-O3,p`, the other SDK units use `-O4,p`, with `-opt nopeep` for the documented fragments. Newlib libc uses ProDG 3.8.1 with `-O2 -G8 -fsigned-char -DMB_CAPABLE`; math uses `-O2 -G1024 -fno-builtin -msafe-sda` and the language profiles in [Newlib.md](docs/Newlib.md). GCC runtime helpers use `-O2 -G8`. The original compiler releases remain unconfirmed.

```sh
python3 tools/import_disc.py "/path/to/Medal of Honor - Rising Sun (USA) (Disc 1).nkit.iso"
python3 tools/setup.py
python3 tools/audit.py
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 -m unittest discover -s tests -v
```

The importer reads the standard GameCube filesystem table, including the provided uncompressed NKit v01 image. It extracts only four pinned executables into `orig/GR8E69/`, checks their sizes and SHA-1/SHA-256 digests, and leaves the input image untouched. This does not recover or verify a full retail-disc image. Other compressed formats and other revisions are not currently supported.

Run the scripts from any directory; all outputs remain inside this checkout. `baseline.py` and `reconstruct.py` recreate their own build directories, so keep experiments under `scratch/`.

## Outputs

| Path | Contents |
| --- | --- |
| `config/GR8E69/target.json` | Executable identities and hashes |
| `config/GR8E69/baseline.json` | Entry point, SDA bases and comparison layout |
| `build/audit/summary.json` | Section and symbol statistics |
| `build/audit/symbols.json` | Complete local symbol inventory |
| `build/audit/dwarf.txt` | Local debug-information dump |
| `build/audit/elf-config/` | Experimental dtk symbol/split exports |
| `build/baseline/report.json` | Verified baseline result, with zero source credit |
| `build/baseline/relinked.dol` | Rebuilt analysis image |
| `build/audit/dependencies.json` | Local direct-branch dependency graph and research ranking (`python3 tools/dependencies.py`) |
| `src/matrix/`, `include/game/CMatrix.h` | 16 reconstructed matrix functions and a shared layout with explicit vector-type limits |
| `src/sys_memory.cpp` | Four game allocation operators; original heap routines remain external |
| `src/stlport/` | 92 tree/container functions using an attributed STLport 4.5.3 subset |
| `src/MathFun.cpp` | Six reconstructed functions from the original MathFun unit |
| `src/lua/` | 19 restored Lua 4.0.1 source units, headers and copyright notice |
| `src/dolphin/`, `include/dolphin-sdk/` | 89 accepted SDK source units, reference headers and attribution |
| `src/newlib/`, `include/newlib/` | 99 accepted Newlib units, supporting headers and attribution |
| `src/libgcc/` | Eight verified GCC 2.95.2 runtime variants, shared source, target configuration and notices |
| `config/GR8E69/MathFun.json` | Function ranges, external references and compiler profile |
| `config/GR8E69/project.json` | Accepted unit list, source provenance and fixed progress denominator |
| `build/reconstruction/report.json` | Full-image result and source/tool fingerprints |
| `build/reconstruction/units/` | Per-unit compiler, linker and generated-section output |
| `build/reconstruction/rebuilt.dol` | Analysis image containing all accepted compiled source |
| `progress/GR8E69.snapshot.json` | Verification metadata, source hashes and unfinished-code map; no original bytes |
| `build/progress/report.json` | Generated objdiff v2 progress for decomp.dev |

The automatic dtk ELF split export emits warnings for this executable's symbol ordering. Treat it as research output; it is not accepted source ownership. The baseline uses a separate original-object split and retains every compared byte.

## Next work

1. Extend the [verified CMatrix family](docs/Matrix.md) using its shared layout; establish CVector3 construction and full storage before adding functions that need local vectors. The [dependency ranking](docs/Dependencies.md) also identifies small string and allocation helpers. Extend the verified STL container profile only after resolving custom comparator and by-value game-type layouts. The remaining MathFun functions need floating-point constant/layout evidence.
2. Distinguish the original compiler release using larger functions. Five tested SN/ProDG versions match the accepted fragment, so its byte match alone cannot identify the original release. Nintendo libraries may use different compilers.
3. Extend the network-library subset by recovering the older target's behavior and storage; the remaining TCP, UDP, PPP and Ethernet reference routines differ. Continue shared game-code reconstruction alongside library restoration. Only compiled source bytes that pass complete-image verification count toward progress.
4. Inspect Disc 2 and test the game's executable-loading path before claiming complete game coverage or a runnable replacement disc.

See [the initial audit](docs/initial-audit.md) for evidence and limitations.

## Tooling

Uses [decomp-toolkit](https://github.com/encounter/decomp-toolkit) 1.8.4, [gc-wii-binutils](https://github.com/encounter/gc-wii-binutils) 2.42-2, and [wibo](https://github.com/decompals/wibo) 1.0.3. The project layout follows the separation of originals, source and build output documented by [dtk-template](https://github.com/encounter/dtk-template); its standard CodeWarrior build configuration has not been adopted for the SN-compiled game code.
