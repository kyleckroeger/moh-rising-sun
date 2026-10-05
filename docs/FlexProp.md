# FlexProp and string CRC evidence

[kyleckroeger’s research offer in issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4) highlighted CRC-derived FlexProp setting names. That useful lead prompted this reconstruction of the GameCube lookup path. The detailed notes and 241-name list were shared after these fragments were reconstructed and are now available in the [PS2 FlexProp research](research/ps2/flexprop-crc-names.md). These bodies and scoped storage views were independently reconstructed from the pinned GR8E69 executable with Codex assistance; no name from that list or PS2 layout is claimed as evidence for the accepted source here.

## Accepted lookup path

`src/flexprop/` reconstructs field search and its comparator, name-to-key wrappers, typed scalar/list access, position access and class ancestry queries. `src/string_crc.cpp` reconstructs their string hash dependency. Together these fragments add 25 functions and 1,552 executable bytes. Individual ranges and generated string/constant storage are recorded in their manifests.

`FlexProp` holds a format pointer at offset zero. The format’s pointer at `+4` leads to a class-description record containing its name at `+0`, parent at `+8`, field count at `+12`, and a variable tail of twelve-byte field records at `+16`. Each field contains a signed CRC key, a type word and a byte offset. The original `bsearch` stride, comparator and field accesses independently establish those three words. The zero-length array in the scoped header represents the variable tail; it is not a complete class allocation.

`FlexPropFormat::GetField` searches each class’s sorted fields, then its parent if no field matches. Its twelve-byte search record initializes only the key, which is the only word the comparator reads. Comparisons use signed 32-bit ordering; CRC bit patterns must retain that ordering when interpreting these field arrays.

`GetDataPtr` returns null for a missing field; otherwise it adds the field offset to the format’s `0x3c`-byte prefix. `GetFieldOffset` returns -1 for a missing field, and `GetData` adds an already supplied byte offset without validation. Integer, enum, float and boolean getters retain original diagnostics and zero/false fallback results. List lookup returns the original shared empty-list object when absent; the script group filters now establish its signed count and variable tail of signed
32-bit values; complete allocation and serialization remain unknown. See
[the script evidence](Script.md#thread-lifecycle-queued-delivery-and-group-filters). The private empty-list symbol is scoped to the original `flexprop.cpp` record and remains uncredited context.

Name-taking wrappers hash the exact input and call their integer-key overloads. The name-taking string getter is accepted, but its integer-key implementation and string-table virtual interface remain original context. `GetFieldType` returns zero for an absent field. `GetClassName` reads the first class name; `IsA` compares names with `strcmp` while following parents. Case folding is not added.

Position accesses confirm floats at format offsets `0x30`, `0x34` and `0x38`, with `SetPositionZ` writing the last. Independent inspection of the original matrix getter supports the twelve-float transform prefix beginning at `0x0c`; that matrix getter is not reconstructed here. CVector3 stays opaque, and only its established three-float prefix is written. Unknown words, complete allocation sizes, original field spelling and historical return-type spelling remain unproven.

## CRC details useful for future name recovery

The matching `GetStringCRC` operates on unsigned string bytes, stops before the terminating zero, and performs no case conversion or other normalization. Null and empty inputs return zero. Nonempty inputs start at `0xffffffff`, update as `table[(byte ^ crc) & 255] ^ (crc >> 8)`, and complement the final result.

The original private `crcTable` is a 256-word read-only object at `0x802b7fc0`, scoped to `crc.cpp`. All 256 original values were independently compared with a table generated from reflected polynomial `0xedb88320`; they agree. The implementation keeps that table external and receives no data credit. This establishes the exact GameCube hash convention, but a matching CRC alone still does not uniquely establish a setting’s spelling or semantics.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every accepted function, diagnostic string and generated constant. No byte patches, inline assembly implementations, discarded functions, clipped sections or fuzzy matches are used. The private comparator and dependencies retain their original symbol binding/file scope. Whole-image verification and the public snapshot checks remain required; runtime behavior has not been emulator-tested.

These declarations and the recipe evidence in [ParticleRecipes.md](ParticleRecipes.md) provide a starting point for applying the [contributed PS2 research](research/README.md). The script/opcode, `.bpd` and music-rule notes record their own GameCube comparisons; they do not add reconstructed source in this batch.
