# Initial executable audit

## Input and target

The supplied uncompressed NKit v01 image identifies itself as GameCube `GR8E69`, disc number 0, revision 0. Its filesystem table contains `MOH3RDVD.ELF`, `MOH3RDC.ELF` and `STUBRDVD.ELF`; the system boot DOL is separate. Each extracted executable is pinned in `config/GR8E69/target.json`. The image is read-only input; no full-disc recovery or retail-disc checksum claim is made.

The initial target is `MOH3RDVD.ELF`, a big-endian PowerPC ELF32 executable with embedded-PowerPC flag `0x80000000` and entry point `0x80035300`. The boot program includes the names `MOH3RDC.ELF`, `MOH3RDVD.ELF` and `MOH3RDO.ELF`. Its selection/loading behavior has not yet been reconstructed or tested.

## Symbols and debug coverage

The ELF symbol table contains 15,507 entries: 9,883 function entries, 2,764 object entries, 954 file entries, 16 section entries and 1,890 entries without a specific type. These counts include aliases and other records; they are not a decompiled-function total.

There are 701 `gcc2_compiled.` markers and GCC-style C++ mangled names. Together with the `SNDebugInit` call from startup, they support investigating the SN/ProDG toolchain family. They do not identify an exact compiler version or optimization settings.

The `.debug` section is 42,460 bytes and `.line` is 3,176 bytes. dtk's DWARF 1.1 dump identifies seven compilation units with producer `MW EABI PPC C-Compiler`: `GBA.c`, `GBAJoyBoot.c`, `GBARead.c`, `GBAWrite.c`, `GBAXfer.c`, `GBAKey.c` and `GBAGetProcessStatus.c`. These are Nintendo library records, not full game debug information.

The dtk automatic ELF configuration emits file-order/ownership warnings. Preserve its output for research only. Do not accept its suggested splits wholesale as recovered original compilation units.

## Baseline scope

The baseline performs the following operations using checksum-pinned tools:

1. Validate the complete original ELF against its recorded size and SHA-256.
2. Convert it to a DOL with dtk 1.8.4 and check the resulting size and SHA-1.
3. Independently compare every allocated file-backed ELF section against the DOL's corresponding address range; verify the entry point and all NOBITS extents.
4. Split the derived DOL into ten original relocatable objects, then link them at the recorded addresses with GNU PowerPC binutils.
5. Convert the linked ELF to a DOL and compare every byte, including the DOL header, against the original derived reference.

The ELF-derived DOL is 2,860,576 bytes with SHA-1 `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. This is not the disc's 206,016-byte boot DOL. The original ELF's debug tables, symbol ordering and file layout are not reconstructed by this baseline.

## Memory-layout evidence

Startup at `0x80035308`–`0x80035314` loads r2=`0x80469220` and r13=`0x80459220`. These supply the SDA2 and SDA bases in the baseline linker configuration.

The original ELF contains an allocated, zero-sized `.sbss2` at `0x804535C0`. dtk includes that endpoint in the DOL BSS span even though the preceding nonempty `.sbss` ends at `0x8045279C`. The splitter omits the empty section. The baseline therefore creates an empty NOBITS input section and explicitly places it at the original `.sbss2` address. It contributes no instructions or stored bytes and preserves the original converted BSS endpoint. Without it, only two header bytes differ; those differences are not ignored or patched.

## Milestone at the initial audit

Import, local symbol/debug inventory and complete original-object relink are verified. Reconstructed source remains **0 bytes**. No compiler candidate has yet been validated, and no emulator, original-console, Disc 2 or full-disc replacement test has been performed.

This section records the initial baseline. See the repository README for current source progress.
