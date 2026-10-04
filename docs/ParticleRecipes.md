# Particle recipe access

The lead for this work came from [kyleckroeger’s offer of PS2 research in issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4), specifically the connection between particle recipes and `CParticleSystem` getters. We appreciate that direction: it led to matching GameCube code and a usable field map. The detailed PS2 notes had not been supplied when these fragments were reconstructed. Every accepted layout and body below was recovered independently from the pinned GR8E69 executable; no PS2 offsets or code were imported. Codex assisted reconstruction and verification.

## Accepted scope

The `src/particles/` fragments reconstruct 41 functions and 2,180 executable bytes: recipe getters, seed and state access, transform wrappers, particle-clock access, procedural-definition validity, emission setters, lifetime handling and position/velocity bound setters. Their exact original names and ranges live in `config/GR8E69/particle_*.json`. The original misspelling `GetEmmisionRate` / `GetEmmisionDelay` is retained.

`include/game/ParticleRecipe.h` supplies scoped runtime storage views, not a complete particle engine or an on-disc `.lfc` parser. Only these accesses are established:

| Storage | Observed access |
| --- | --- |
| `CParticleSystem` | Matrix at `0x90`; definition pointer at `0x140`; deactivation tick value at `0x150`; unsigned seed at `0x160`; flag word at `0x170` |
| `CProcParticleDef` | Contents pointer at `0x04`; leading word remains unknown |
| `LevelFileContentsStruct_` | Pointer to an entry-pointer array at `0x08`; first two words remain unknown |
| Recipe entry | Integer or float value at `0x08`; first two words remain unknown |

The recipe-entry union describes the two access types, not a recovered historical union declaration. The system/definition prefixes deliberately omit inheritance, virtual dispatch and remaining fields. Do not allocate objects or infer their full size from these views. Return-type spelling, field/helper names and visibility are reconstruction choices; method names and parameter encodings come from original symbols. CVector3 remains opaque: output helpers write only its already established first three floats.

## GameCube recipe indices

These are zero-based positions in the runtime entry-pointer array. Numeric indices and accessed values are supported directly by matching getters, and paired setters independently corroborate emission, lifetime and bounds. They are not recovered field-name strings, file offsets, timing units or enum definitions.

| Index | Access | Meaning supported by these functions |
| --- | --- | --- |
| 3 | int | Emission rate |
| 4 | int | Emission delay |
| 5 | float | System lifetime |
| 7–9 / 10–12 | float triples | Minimum / maximum particle position |
| 13–15 / 16–18 | float triples | Minimum / maximum particle velocity |
| 22–24 / 39–41 / 26–28 / 42–44 | int triples, narrowed to bytes | First / second / third / fourth RGB outputs of `GetParticleColor` |
| 25 / 45 / 29 | int, converted to float | First / second / third outputs of `GetParticleAlpha` |
| 30 | int | Render-type value; enum members unknown |
| 31, 33 / 32, 34 | float pairs | First / second size-vector X and Y outputs; both Z outputs are zero |
| 35 | float | Additional output of `GetParticleSize`; exact role unconfirmed |
| 36 | float | Particle lifetime |
| 37 / 38 | float | First / second rotation outputs |
| 46 | int | No-fog flag: `GetFogEnable` returns one when this entry is zero |
| 47 / 48 | float | First / second fade outputs |

Emission setters clamp nonpositive integers to one. The lifetime setter uses `!(value >= 0.0f)`, so negative and unordered inputs become zero while negative zero is retained. Position/velocity setters first store the requested triple and then adjust the opposite bounds using negated `>=` or `<=` comparisons. Their unordered-input behavior and repeated pointer reloads are preserved. No null checks, additional bounds checks or runtime fixes have been added.

The size getter preserves its three-float writes and alias-sensitive ordering. Seed methods access the system directly. The particle lifetime setter delegates to the original named procedural-definition method; its implementation is separately accepted in this batch. The fog result is modeled as a normalized integer flag, matching its emitted ABI and inversion.

The alpha getter converts signed integers to floats without normalization or clamping. Its compiler-generated eight-byte integer-conversion constant is checked at `0x80299cc0`. The color getter takes the low byte of each integer component and writes four packed colors with alpha `0xff`. It constructs and copies each output before looking up the next, preserving alias-sensitive ordering. `CColor` remains opaque: the local `ColorPrefix` union describes only the four accessed bytes. Its signed alpha member supplies the observed `-1` initialization; it does not establish the original member's signedness. Independent inspection of `Color(const CColor&)` and `CFont::SetColor(CColor)` corroborates the four-byte color prefix, without establishing the complete historical class.

## State, transforms and clock

The state accessors establish the following bits in the word at `+0x170`, using conventional least-significant-bit numbering. The scoped bitfield view follows the target compiler's big-endian allocation order; other bits remain explicitly unknown. Names and the declaration are reconstruction choices supported by the accesses, not a recovered historical header.

| Bit | Access |
| --- | --- |
| 30 | Active: `Start` sets it; `DeActivate` clears it; `IsActive` reads it |
| 28 | `IsMoving` |
| 27 | `IsRotating` |
| 26 | `IsDestroyable` / `SetDestroyable` |
| 25 | `IsDying` |
| 24 | `UseFade` |
| 23 | `IsEternal` |
| 22 | `Profile(bool)` writes it |

The setters preserve all other bits. Flag getters are represented with normalized unsigned integer returns, matching the observed ABI; original return-type spelling is unproven. `GetDef` exposes the already modeled definition pointer.

Both `GetLocalToWorld` and `GetTMLocalToWorld` assign the matrix at `+0x90` through the accepted `CMatrix::operator=`. The particle `Orthonormalize` wrapper passes that same matrix to the original `CMatrix::Orthonormalize` body, which remains external context. These independent accesses establish the matrix's placement and use the separately verified 64-byte matrix representation. No missing constructor or complete particle inheritance layout is supplied.

`DeActivate` clears the active bit and then stores `CPSManager::GetCurrentTicks()` at `+0x150`. The scoped `ParticleClock.h` interface reconstructs two static manager methods: `Update(float)` adds its argument to the shared float, and `GetCurrentTicks()` reads it. The original private `g_numCurrentTicks` at `0x802c1c44` is scoped to the `particlesystemmanager.cpp` symbol record and remains external storage, with no source/data credit. Tick units, initialization policy and full manager storage are not inferred from these methods.

## Verification and remaining work

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` matches every allocated byte in each accepted fragment, including the two four-byte zero constants and eight-byte integer-conversion constant. This is a working profile, not proof of the original compiler release. All fragments also pass the complete rebuilt analysis-image comparison and snapshot safeguards; constants and original dependencies earn no code credit. Runtime and emulator behavior remain untested.

Cached vector getters, construction, simulation and rendering remain outside this batch. The contributor’s [PS2 particle recipe notes](research/ps2/particle-recipes.md) are now available to help interpret additional fields and names, but each cross-platform claim still needs a separate GR8E69 check. These matched accessors provide concrete locations for those checks.
