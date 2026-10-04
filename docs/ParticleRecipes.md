# Particle recipe access

The lead for this work came from [kyleckroeger’s offer of PS2 research in issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4), specifically the connection between particle recipes and `CParticleSystem` getters. We appreciate that direction: it led to matching GameCube code and a usable field map. The detailed PS2 notes had not been supplied when these fragments were reconstructed. Every accepted layout and body below was recovered independently from the pinned GR8E69 executable; no PS2 offsets or code were imported. Codex assisted reconstruction and verification.

## Accepted scope

The `src/particles/` fragments reconstruct 22 functions and 1,360 executable bytes: recipe getters, seed access, procedural-definition validity, emission setters, lifetime handling and position/velocity bound setters. Their exact original names and ranges live in `config/GR8E69/particle_*.json`. The original misspelling `GetEmmisionRate` / `GetEmmisionDelay` is retained.

`include/game/ParticleRecipe.h` supplies scoped runtime storage views, not a complete particle engine or an on-disc `.lfc` parser. Only these accesses are established:

| Storage | Observed access |
| --- | --- |
| `CParticleSystem` | Definition pointer at `0x140`; unsigned seed at `0x160` |
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
| 30 | int | Render-type value; enum members unknown |
| 31, 33 / 32, 34 | float pairs | First / second size-vector X and Y outputs; both Z outputs are zero |
| 35 | float | Additional output of `GetParticleSize`; exact role unconfirmed |
| 36 | float | Particle lifetime |
| 37 / 38 | float | First / second rotation outputs |
| 46 | int | No-fog flag: `GetFogEnable` returns one when this entry is zero |
| 47 / 48 | float | First / second fade outputs |

Emission setters clamp nonpositive integers to one. The lifetime setter uses `!(value >= 0.0f)`, so negative and unordered inputs become zero while negative zero is retained. Position/velocity setters first store the requested triple and then adjust the opposite bounds using negated `>=` or `<=` comparisons. Their unordered-input behavior and repeated pointer reloads are preserved. No null checks, additional bounds checks or runtime fixes have been added.

The size getter preserves its three-float writes and alias-sensitive ordering. Seed methods access the system directly. The particle lifetime setter delegates to the original named procedural-definition method; its implementation is separately accepted in this batch. The fog result is modeled as a normalized integer flag, matching its emitted ABI and inversion.

## Verification and remaining work

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` matches every allocated byte in each accepted fragment, including the two four-byte zero constants. This is a working profile, not proof of the original compiler release. All fragments also pass the complete rebuilt analysis-image comparison and snapshot safeguards; constants and original dependencies earn no code credit. Runtime and emulator behavior remain untested.

Color/alpha access, cached vector getters, construction, simulation and rendering remain outside this batch. The contributor’s future PS2 notes can help interpret additional fields and names, but each cross-platform claim still needs a separate GR8E69 check. These matched accessors provide concrete locations for those checks.
