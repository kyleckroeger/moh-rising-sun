# PS2 research: FlexProp setting names stored as CRCs

Research notes from the PlayStation 2 release, offered in
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4). They come from
kyleckroeger's own investigation of the PS2 files for a separate remake project, were
written with AI assistance (Claude), and were re-checked against the files on 2026-10-04
as described below. No game files, extracted data or bulk dumps are included; the
names below are short identifiers whose CRCs were matched, and every CRC was computed
from the name, not copied from the game.

Nothing here is reconstructed source and nothing earns progress credit. GameCube status
is given per fact; "unverified" means nobody has checked it against GR8E69 yet.

## Build and files examined

| | |
|---|---|
| Release | Medal of Honor: Rising Sun, PS2, US, game ID **SLUS-20753** (`SYSTEM.CNF` boots `SLUS_207.53`) |
| `SLUS_207.53` | 118,724 bytes, SHA-1 `ba2dc72f12195e86e08977784e726534c54111c1` |
| `MOH3RDVD.ELF` (same disc) | 2,692,080 bytes, SHA-1 `2d524fe5a0c9c452da60635e906670d30c556d63`; one load segment, file offset `0x80` at address `0x135480`, so **file offset = address − `0x135400`**. All PS2 addresses below are addresses, not file offsets. |
| Level data | `1_1_P.bpd` in `DATA/1/1_1/LEVEL.VIV` and `1_11_P.bpd` in `DATA/1/1_11/LEVEL.VIV` (only these two levels were examined) |
| Script data | `bsbifunc.dat` (583 entries) |
| GameCube comparison | pinned GR8E69 executables in `orig/GR8E69/`, digests checked against `config/GR8E69/target.json` before use; upstream [FlexProp.md](../../FlexProp.md) at `bcbefb0` |

Sources are labelled **[notes]** (the contributor's earlier research notes, 2026-10-03/04),
**[re-check]** (re-done for this document with small read-only scripts over the files
above) and **[upstream]** (this repository's own GameCube findings).
Confidence: **high** means the bytes were checked directly and the result is consistent
across all examined files, **medium** means it is consistent but there's an untested
alternative, and **low** means it's an interpretation.

## Two hash conventions

| Used for | Input | Algorithm | Example |
|---|---|---|---|
| Setting (FlexProp field) names in `.bpd` class layouts and in script reads | the name **exactly as spelled**, case preserved, ASCII bytes, no terminator | standard CRC-32: reflected polynomial `0xEDB88320`, initial value `0xFFFFFFFF`, final complement (identical to Python `zlib.crc32`) | `zlib.crc32(b"Radius")` |
| Built-in script function (BIF) names in `bsbifunc.dat` | the name converted to **upper case** | the same CRC-32 | `GetFlexProperty` → `zlib.crc32(b"GETFLEXPROPERTY")` = `0x10f65ab2` |

- **Settings, exact case:** [notes] [re-check], confidence **high**. All 241 names
  below were matched with exact-case input; none needed case changes. GameCube status:
  **checked**. [upstream] documents that GR8E69's `GetStringCRC` computes this same
  CRC with no case conversion, and returns 0 for null or empty input. `zlib.crc32(b"")`
  is also 0.
- **BIFs, upper case:** [re-check], confidence **high**. All 583 `bsbifunc.dat` CRCs
  equal the upper-case CRC-32 of a `BIFunc_<name>` symbol name in the GR8E69
  `MOH3RDVD.ELF`, and **none** match with exact case. Each entry is 16 bytes from file
  offset `0x14`, with the CRC as the second word. For example `GetFlexProperty` is entry
  433, at file offset `0x1b24`. The PS2 engine's own table at address `0x35D028`
  (pairs of CRC and code address, in the same order) holds the same CRC at entry 433
  [notes] [re-check]. GameCube status: the names come **from** GR8E69 symbols, but where
  GR8E69 upper-cases BIF names at run time is **unverified**.

A matching CRC alone does not prove a spelling, as upstream also notes; see the
"Name status" section below.

## Where the setting CRCs live (PS2)

**Class layout table**, in each `.bpd`: [notes] [re-check], confidence **high** (all
classes in both levels parse to the end of the table). The header words at `0x40` and
`0x44` give the table's file offset and class count (92 classes in `1_1`, 54 in `1_11`).
Each class has:

| Bytes | Contents |
|---|---|
| 4 | class name (index into the file's string table) |
| 4 | data size |
| 4 | parent class (string index, 0 = none); the parent's settings come first |
| 4 | setting count *n*, followed by *n* records of 12 bytes: |
| 4 | setting name CRC-32 (exact case) |
| 4 | type code |
| 4 | byte offset of the value |

GameCube status: **consistent, not checked against GameCube data files**. [upstream]
describes GR8E69 FlexProp field records as 12 bytes: signed CRC key, type word, byte
offset. Those are the same three words in the same order.

**Sort order:** [re-check], confidence **high**. In all 146 classes across the two
levels, the setting records are sorted by the CRC read as a **signed** 32-bit integer.
Read as unsigned, only 53 classes are sorted. GameCube status: **consistent**.
[upstream] found that `FlexPropFormat::GetField` binary-searches each class's fields with
signed comparison, then falls back to the parent class.

**Value location:** [notes], confidence **high** for the offset arithmetic. A value is at
byte `60 + offset` of the object record. The record starts with name, class and size
words (12 bytes), then 9 rotation floats at `0x0c` and a position at `0x30`, `0x34`
and `0x38`; settings start at `0x3c`. GameCube status: **consistent**. [upstream]'s
`GetDataPtr` adds the field offset to a `0x3c`-byte prefix, with position floats at
`0x30`/`0x34`/`0x38` and a twelve-float transform starting at `0x0c`.

**Type codes:** [notes]. 1 integer, 4 float, 5 boolean, 6 list, 7 enum and 8 string
index are **medium** confidence, based on the values seen. 2 (script name), 9 (animation)
and 10 (motion) are **low**: all three hold string indices and the labels are guesses. A
list value is an absolute file offset of a count followed by the items. An enum holds a
small number or a CRC of an option name. GameCube status: **unverified**; the GR8E69
type-word values have not been compared.

**Script reads:** [notes], confidence **medium**. Behaviour scripts read settings through
the BIF `GetFlexProperty` using the same exact-case CRCs. GameCube status:
**unverified**.

## Name status

Coverage [re-check]: the two levels use **384** distinct setting CRCs. **241** have a
name and **143** are still unknown. Every listed name's CRC occurs in these level files,
and no two listed names share a CRC.

How the names were found [notes]: by hashing guesses (strings from the PS2 program and
level files, plus combinations of one to three words) and keeping exact matches. 17
names came from words in GR8E69 symbol names. Chance matches were removed by hand where
they were obviously not names. The per-name origin was not recorded, so it can't be
given below.

- **Confirmed spelling** (42 names): the exact name exists as a whole, zero-terminated
  string in the PS2 and/or GR8E69 `MOH3RDVD.ELF` **and** its CRC is in the level data.
  A random string's chance of hitting one of 384 keys is about 384 / 2³² ≈ 1 in 11
  million. So even across all strings in a program, a chance spelling match is very
  unlikely (rough estimate, not measured). This confirms the spelling only; it does not
  establish what the setting means. Short generic strings (`ID`, `Dump`, `Hide`, `size`)
  could be present for unrelated reasons, but the exact-case CRC match is still required.
- **Candidate** (199 names): CRC match only. With a large guess space, a few chance
  matches are expected, so treat these as leads to confirm, for example from GameCube
  strings, debug output, or how a script uses the value.

GameCube status of the names: the 42 strings were searched for in the GR8E69
`MOH3RDVD.ELF`. 40 occur in both executables, `Particles` and `size` occur only in
GR8E69, and none occur only on PS2. Whether GameCube level data uses the same keys and
layouts is **unverified**.

To check a name: `zlib.crc32(name.encode("ascii"))`, then compare the result as a signed
or unsigned 32-bit value, matching how the field array is read.

## Open questions

- The 143 unnamed setting CRCs, and settings in levels other than `1_1` and `1_11`.
- The PS2 address of the string-CRC routine itself was not located for this note.
- Whether the GR8E69 type-word values match the PS2 codes above.
- Meanings of individual settings are not covered here; they come from script use and are
  left for a later note.

## Setting names (241)

Sorted alphabetically (ignoring case). "Exact string found in" lists the executables
where the name occurs as a whole zero-terminated string.

| Name (exact spelling) | CRC-32 | As signed int32 | Status | Exact string found in |
|---|---|---|---|---|
| `Accuracy` | `0x6a3361d5` | 1781752277 | confirmed spelling | PS2 + GC |
| `Action` | `0x406089a4` | 1080068516 | candidate | — |
| `ActionIndex` | `0xecea0cc7` | -320205625 | candidate | — |
| `ActivatedState` | `0xc42f1058` | -1003548584 | candidate | — |
| `ActivateOnStart` | `0x9e0864df` | -1643617057 | candidate | — |
| `ActiveFrame` | `0x41e07ed5` | 1105231573 | candidate | — |
| `AddObjective01` | `0x19e8d8af` | 434690223 | candidate | — |
| `AddObjective02` | `0x80e18915` | -2132702955 | candidate | — |
| `AddObjective03` | `0xf7e6b983` | -135874173 | candidate | — |
| `AddObjective04` | `0x69822c20` | 1770138656 | candidate | — |
| `AddObjective05` | `0x1e851cb6` | 512040118 | candidate | — |
| `AddObjective06` | `0x878c4d0c` | -2020848372 | candidate | — |
| `AddObjective07` | `0xf08b7d9a` | -259293798 | candidate | — |
| `AddObjective08` | `0x6034600b` | 1614045195 | candidate | — |
| `AddObjective09` | `0x1733509d` | 389238941 | candidate | — |
| `AddObjective10` | `0x77f4d978` | 2012535160 | candidate | — |
| `AftSpray` | `0xee0611d5` | -301592107 | candidate | — |
| `Aggression` | `0x9b814039` | -1686028231 | confirmed spelling | PS2 + GC |
| `AIState` | `0xf337f0f5` | -214437643 | candidate | — |
| `alias` | `0xe16c6b94` | -512988268 | candidate | — |
| `AliveParticles1` | `0xfe7e3aa4` | -25281884 | candidate | — |
| `AliveParticles2` | `0x67776b1e` | 1735879454 | candidate | — |
| `AmbienceAzimuth` | `0x1843ed20` | 407104800 | candidate | — |
| `AmbienceVolume` | `0x8053b719` | -2141997287 | candidate | — |
| `AmbientTrack` | `0x6cb0f372` | 1823535986 | candidate | — |
| `AmmoCount` | `0xf5f7ba51` | -168314287 | candidate | — |
| `anim` | `0x67ab645b` | 1739285595 | candidate | — |
| `AnimGraph` | `0x4e637914` | 1315141908 | candidate | — |
| `AttackDistance` | `0xa5760d95` | -1518989931 | confirmed spelling | PS2 + GC |
| `BankID` | `0x9871810d` | -1737391859 | candidate | — |
| `BeginAlpha` | `0x4ecc54cd` | 1322013901 | candidate | — |
| `BeginFadeIn` | `0xf2f8d8b3` | -218572621 | candidate | — |
| `BeginFadeOut` | `0x7a4bf76e` | 2051798894 | candidate | — |
| `Behavior` | `0xc2ddc2e6` | -1025654042 | candidate | — |
| `Blue` | `0x3e04658a` | 1040475530 | candidate | — |
| `BlurAmount` | `0x641a477a` | 1679443834 | candidate | — |
| `BoxX` | `0xd184e98f` | -779818609 | confirmed spelling | PS2 + GC |
| `BoxY` | `0xa683d919` | -1501308647 | confirmed spelling | PS2 + GC |
| `BoxZ` | `0x3f8a88a3` | 1066043555 | confirmed spelling | PS2 + GC |
| `CanDodge` | `0x4b83218b` | 1266885003 | candidate | — |
| `CanKneel` | `0x3b47612e` | 994533678 | candidate | — |
| `ChangeAz` | `0x2e9c2d21` | 781987105 | candidate | — |
| `ChangeTrack` | `0xaf4c61d5` | -1353948715 | candidate | — |
| `ChangeVolume` | `0x70ef42c1` | 1894728385 | candidate | — |
| `ChargePowerupX` | `0x3e4bda7b` | 1045158523 | candidate | — |
| `ChargePowerupY` | `0x494ceaed` | 1229777645 | candidate | — |
| `ChargePowerupZ` | `0xd045bb57` | -800736425 | candidate | — |
| `ChildType` | `0x021aa0f3` | 35299571 | confirmed spelling | PS2 + GC |
| `ClipDistance` | `0x3b716c41` | 997289025 | candidate | — |
| `CollisionOn` | `0xc28f2b34` | -1030804684 | candidate | — |
| `Compartment1` | `0xe9254d5b` | -383431333 | candidate | — |
| `Compartment2` | `0x702c1ce1` | 1881939169 | candidate | — |
| `Compartment3` | `0x072b2c77` | 120269943 | candidate | — |
| `Compartment4` | `0x994fb9d4` | -1722828332 | candidate | — |
| `CompartmentID` | `0xb80534cb` | -1207618357 | candidate | — |
| `CustomAnim` | `0x68162a0e` | 1746283022 | candidate | — |
| `CylinderBottom` | `0xc5af0932` | -978384590 | confirmed spelling | PS2 + GC |
| `CylinderTop` | `0x5a5fc396` | 1516225430 | confirmed spelling | PS2 + GC |
| `DamageDelay` | `0x210fc619` | 554681881 | candidate | — |
| `DamagePerHit` | `0xb4fff2b1` | -1258294607 | candidate | — |
| `DeathAnimation` | `0x4ce43f23` | 1290026787 | candidate | — |
| `DeathCount` | `0x00d44e16` | 13913622 | candidate | — |
| `DeathParticles1` | `0x1e037b9a` | 503544730 | candidate | — |
| `DeathParticles2` | `0x870a2a20` | -2029376992 | candidate | — |
| `DeathTimer` | `0xefddd46e` | -270674834 | candidate | — |
| `DefaultValue` | `0xdc53e7d5` | -598480939 | candidate | — |
| `Delay1` | `0xe31f3acd` | -484492595 | candidate | — |
| `Delay2` | `0x7a166b77` | 2048289655 | candidate | — |
| `DelayInSeconds` | `0xe67b4692` | -428128622 | candidate | — |
| `DisplayPrompt` | `0x5d55d58c` | 1565906316 | candidate | — |
| `Distance` | `0xe5e4f8d7` | -437978921 | candidate | — |
| `Dodge` | `0x50aee58a` | 1353639306 | confirmed spelling | PS2 + GC |
| `Dump` | `0xe75ede1b` | -413213157 | confirmed spelling | PS2 + GC |
| `DumpCount` | `0xe2be65ce` | -490838578 | confirmed spelling | PS2 + GC |
| `Duration` | `0x7f29e296` | 2133451414 | candidate | — |
| `EffectDuration` | `0xd1cf061f` | -774961633 | candidate | — |
| `EnableTracking` | `0xb2fcc61d` | -1292057059 | candidate | — |
| `EndAlpha` | `0x9bb08867` | -1682929561 | candidate | — |
| `EndFadeIn` | `0xc42eee99` | -1003557223 | candidate | — |
| `EndFadeOut` | `0xa1c6e88e` | -1580799858 | candidate | — |
| `eRuntimeBinding` | `0x511e901e` | 1360957470 | confirmed spelling | PS2 + GC |
| `EventID` | `0xdffda238` | -537025992 | candidate | — |
| `ExplodeSound` | `0x0a8c1d84` | 176954756 | candidate | — |
| `Fade` | `0xea67429e` | -362331490 | candidate | — |
| `FadeMS` | `0x817c4eb6` | -2122559818 | candidate | — |
| `FadeType` | `0x680ec128` | 1745797416 | candidate | — |
| `FarDistance` | `0x35ccc1e3` | 902611427 | candidate | — |
| `FileName` | `0x654f240d` | 1699685389 | candidate | — |
| `FilmID` | `0x9d143ce3` | -1659618077 | candidate | — |
| `FireHealth` | `0xcfa15a47` | -811509177 | candidate | — |
| `FireParticles1` | `0x9743792d` | -1757185747 | candidate | — |
| `FireParticles2` | `0x0e4a2897` | 239741079 | candidate | — |
| `FiringTime` | `0x87bb9aa5` | -2017748315 | candidate | — |
| `FramesPerSecond` | `0xfc692a0b` | -60216821 | candidate | — |
| `GagName` | `0x2b2fed0f` | 724561167 | candidate | — |
| `GagScript` | `0xa4458b81` | -1538946175 | candidate | — |
| `GiveCheat` | `0xc08ca175` | -1064525451 | candidate | — |
| `GiveItemPrompt` | `0xff561fca` | -11132982 | candidate | — |
| `GoCode` | `0x4e9e3b51` | 1318992721 | candidate | — |
| `Gravity` | `0x95a01c3b` | -1784669125 | candidate | — |
| `Green` | `0x115bc125` | 291225893 | candidate | — |
| `GrenadeSlot` | `0x77d33c22` | 2010332194 | confirmed spelling | PS2 + GC |
| `Group` | `0xac016bc1` | -1409193023 | confirmed spelling | PS2 + GC |
| `HasCollision` | `0xfeabc5b1` | -22297167 | candidate | — |
| `HearingRadiusAlert` | `0x0d0f8020` | 219119648 | candidate | — |
| `HearingRadiusIdle` | `0xf4362901` | -197777151 | candidate | — |
| `Hide` | `0x04ab6415` | 78341141 | confirmed spelling | PS2 + GC |
| `HitPoints` | `0xc514d0a5` | -988491611 | confirmed spelling | PS2 + GC |
| `ID` | `0x11d3633a` | 299066170 | confirmed spelling | PS2 + GC |
| `IdleAnimation` | `0x9a9253ac` | -1701686356 | candidate | — |
| `IgnoreProximity` | `0xc3539091` | -1017933679 | candidate | — |
| `ImpactParticles` | `0xc747b6c8` | -951601464 | candidate | — |
| `ImpactSound` | `0xdbf97884` | -604407676 | candidate | — |
| `InactiveFrame` | `0x58f42239` | 1492394553 | candidate | — |
| `InactiveState` | `0x4e9fcc0f` | 1319095311 | candidate | — |
| `IncomingStartState` | `0xb7bf2fba` | -1212207174 | confirmed spelling | PS2 + GC |
| `Intensity` | `0x6b433a31` | 1799567921 | candidate | — |
| `Japanese` | `0xf3e12338` | -203349192 | confirmed spelling | PS2 + GC |
| `LetterBox` | `0x9171b66f` | -1854818705 | candidate | — |
| `LightID` | `0xe74e5b19` | -414295271 | candidate | — |
| `Loop` | `0x016db2d0` | 23966416 | candidate | — |
| `Message` | `0x790009e3` | 2030045667 | confirmed spelling | PS2 + GC |
| `Messages` | `0x22747cc0` | 578059456 | candidate | — |
| `MinHitPoints` | `0x12b32804` | 313731076 | candidate | — |
| `MMGLink` | `0x567fbfff` | 1451212799 | confirmed spelling | PS2 + GC |
| `MotionIsPlayerRelative` | `0xd26ed299` | -764489063 | confirmed spelling | PS2 + GC |
| `MoveStyle` | `0x23c05962` | 599808354 | candidate | — |
| `MovieClipPlane` | `0x6a5cc9c3` | 1784465859 | candidate | — |
| `MovieFOV` | `0x6e1b9ef1` | 1847303921 | candidate | — |
| `MovingSound` | `0x2bbfa8f1` | 733980913 | candidate | — |
| `MusicPercent` | `0xdad14627` | -623819225 | candidate | — |
| `NavPoint` | `0x19c98d06` | 432639238 | candidate | — |
| `NearDistance` | `0xe3b23b58` | -474858664 | candidate | — |
| `NonVisDestruct` | `0xb8a775de` | -1196984866 | confirmed spelling | PS2 + GC |
| `NonVisDestructTime` | `0x7feb42fa` | 2146124538 | confirmed spelling | PS2 + GC |
| `Objective01` | `0x9a7ce1bc` | -1703091780 | candidate | — |
| `Objective02` | `0x0375b006` | 58044422 | candidate | — |
| `Objective03` | `0x74728090` | 1953661072 | candidate | — |
| `Objective04` | `0xea161533` | -367651533 | candidate | — |
| `Objective05` | `0x9d1125a5` | -1659820635 | candidate | — |
| `Objective06` | `0x0418741f` | 68711455 | candidate | — |
| `Objective07` | `0x731f4489` | 1931429001 | candidate | — |
| `Objective08` | `0xe3a05918` | -476030696 | candidate | — |
| `Objective09` | `0x94a7698e` | -1800967794 | candidate | — |
| `Objective10` | `0xf460e06b` | -194977685 | candidate | — |
| `OnActivate` | `0x32a8d7ff` | 849926143 | candidate | — |
| `OneShot` | `0x6672b460` | 1718793312 | candidate | — |
| `OneShotSound` | `0x56bf321f` | 1455370783 | candidate | — |
| `OnLookMessages` | `0x7d4a978c` | 2102040460 | candidate | — |
| `OnNPCEnter` | `0x300737b0` | 805779376 | candidate | — |
| `OnNPCExit` | `0x30d5ba97` | 819313303 | candidate | — |
| `OnPlayerEnter` | `0x3e7f4c73` | 1048530035 | candidate | — |
| `OnPlayerExit` | `0x93264b9f` | -1826206817 | candidate | — |
| `Particle01` | `0xa7c93dba` | -1479983686 | candidate | — |
| `Particle02` | `0x3ec06c00` | 1052797952 | candidate | — |
| `Particles` | `0x4ea4bbd9` | 1319418841 | confirmed spelling | GC |
| `PathfinderLatency` | `0x7b5eb2c0` | 2069803712 | candidate | — |
| `PathID` | `0xeb8d2b3f` | -343069889 | confirmed spelling | PS2 + GC |
| `PathTraversal` | `0xf6022acd` | -167630131 | confirmed spelling | PS2 + GC |
| `PatrolSpeed` | `0x642bd270` | 1680593520 | candidate | — |
| `PickupSound` | `0x9cbbab2e` | -1665422546 | candidate | — |
| `PlaneType` | `0x505a1661` | 1348081249 | candidate | — |
| `PortSpray` | `0x601253dd` | 1611813853 | candidate | — |
| `Prompt01` | `0x65acb507` | 1705817351 | candidate | — |
| `Prompt02` | `0xfca5e4bd` | -56236867 | candidate | — |
| `Prompt03` | `0x8ba2d42b` | -1952263125 | candidate | — |
| `Prompt04` | `0x15c64188` | 365314440 | candidate | — |
| `Prompt05` | `0x62c1711e` | 1656844574 | candidate | — |
| `Prompt06` | `0xfbc820a4` | -70770524 | candidate | — |
| `Prompt07` | `0x8ccf1032` | -1932586958 | candidate | — |
| `Prompt08` | `0x1c700da3` | 477105571 | candidate | — |
| `Prompt09` | `0x6b773d35` | 1802976565 | candidate | — |
| `Prompt10` | `0x0bb0b4d0` | 196130000 | candidate | — |
| `PromptID` | `0xbb3ef20b` | -1153502709 | candidate | — |
| `PromptID1` | `0x14b508cd` | 347408589 | candidate | — |
| `PromptID10` | `0x111ad424` | 286970916 | candidate | — |
| `PromptID2` | `0x8dbc5977` | -1917036169 | candidate | — |
| `PromptID3` | `0xfabb69e1` | -88380959 | candidate | — |
| `PromptID4` | `0x64dffc42` | 1692400706 | candidate | — |
| `PromptID5` | `0x13d8ccd4` | 332975316 | candidate | — |
| `PromptID6` | `0x8ad19d6e` | -1965974162 | candidate | — |
| `PromptID7` | `0xfdd6adf8` | -36262408 | candidate | — |
| `PromptID8` | `0x6d69b069` | 1835642985 | candidate | — |
| `PromptID9` | `0x1a6e80ff` | 443449599 | candidate | — |
| `PromptPickupFirst` | `0xf6d4135c` | -153873572 | candidate | — |
| `PromptPickupSecond` | `0x21a96d1c` | 564751644 | candidate | — |
| `Prop` | `0x0f5c6ad4` | 257714900 | confirmed spelling | PS2 + GC |
| `Radius` | `0x3cd06b6c` | 1020291948 | confirmed spelling | PS2 + GC |
| `RateOfFire` | `0xd5331475` | -718072715 | confirmed spelling | PS2 + GC |
| `ReactionDelay` | `0xe54b0ac6` | -448066874 | confirmed spelling | PS2 + GC |
| `Red` | `0xc22c196f` | -1037297297 | candidate | — |
| `ReplayItem` | `0x01de4694` | 31344276 | candidate | — |
| `ReplayLandScript` | `0x45ac4ad2` | 1168919250 | candidate | — |
| `Retrigger` | `0xa5a103a2` | -1516174430 | confirmed spelling | PS2 + GC |
| `RetriggerTime` | `0x3b228263` | 992117347 | confirmed spelling | PS2 + GC |
| `SavePointID` | `0x3912017a` | 957481338 | confirmed spelling | PS2 + GC |
| `Seconds` | `0xe06b307a` | -529846150 | candidate | — |
| `SendEvent` | `0x417c6f88` | 1098674056 | candidate | — |
| `SendMessageOnDeath` | `0x60c71c19` | 1623661593 | candidate | — |
| `SetFade` | `0x10196930` | 270100784 | candidate | — |
| `SetLatency` | `0xeaae9ba6` | -357655642 | candidate | — |
| `SetLevel` | `0x6abf13d3` | 1790907347 | candidate | — |
| `ShakeSound1` | `0x886386dd` | -2006743331 | candidate | — |
| `ShakeSound2` | `0x116ad767` | 292214631 | candidate | — |
| `ShakeType` | `0xbd50b5ed` | -1118784019 | candidate | — |
| `ShowPrompt` | `0xa297e29e` | -1567104354 | candidate | — |
| `size` | `0xf7c0246a` | -138402710 | confirmed spelling | GC |
| `SmokeParticles` | `0x755eafe7` | 1969139687 | candidate | — |
| `SoftSave` | `0xc59af7f4` | -979699724 | candidate | — |
| `Sound` | `0x394fec80` | 961539200 | confirmed spelling | PS2 + GC |
| `Sound1` | `0x6e5d237b` | 1851597691 | candidate | — |
| `Sound2` | `0xf75472c1` | -145460543 | candidate | — |
| `SoundTimer` | `0xf31cf84b` | -216205237 | candidate | — |
| `SparkParticles` | `0xa43670a0` | -1539936096 | candidate | — |
| `SquadLeader` | `0x0951e464` | 156361828 | candidate | — |
| `StartActive` | `0x3543808e` | 893616270 | candidate | — |
| `StartEnabled` | `0xb48929f3` | -1266079245 | candidate | — |
| `StartGag` | `0xd8aeb2b7` | -659639625 | candidate | — |
| `StartState` | `0x4f93e3f6` | 1335092214 | candidate | — |
| `StartTransitionTime` | `0x7ae37dbe` | 2061729214 | candidate | — |
| `StopTransitionTime` | `0x006af416` | 7009302 | candidate | — |
| `Team` | `0x64d20921` | 1691486497 | confirmed spelling | PS2 + GC |
| `TrackingDelay` | `0xcf2f06b4` | -819001676 | confirmed spelling | PS2 + GC |
| `TransitionDuration` | `0xcd8a472d` | -846575827 | candidate | — |
| `TransitionTime` | `0x6e296a2b` | 1848207915 | candidate | — |
| `Turret` | `0xce4e7a13` | -833717741 | confirmed spelling | PS2 + GC |
| `UsedItemPrompt` | `0x4cdac73b` | 1289406267 | candidate | — |
| `UseFrames` | `0x6d9dc809` | 1839056905 | candidate | — |
| `UseMotionBlur` | `0xf41fb455` | -199248811 | candidate | — |
| `UseMovieCamera` | `0xe00df46d` | -535956371 | candidate | — |
| `UseSpecificGroup` | `0xd6942629` | -694933975 | candidate | — |
| `UseStopAnimation` | `0x0e271b35` | 237443893 | candidate | — |
| `VisionDepthAlert` | `0xc558a540` | -984046272 | candidate | — |
| `VisionDepthAttack` | `0xddad3b6e` | -575849618 | candidate | — |
| `VisionDepthIdle` | `0x7442c933` | 1950533939 | candidate | — |
| `VoiceType` | `0x22be35b1` | 582890929 | candidate | — |
| `VolumeType` | `0x055e59c6` | 90069446 | confirmed spelling | PS2 + GC |
| `WakeParticles` | `0x24ec26f3` | 619456243 | candidate | — |
| `WakeTimer` | `0x50c018c1` | 1354766529 | candidate | — |
| `WaterLevel` | `0xd92d2448` | -651353016 | candidate | — |
| `WeaponSlot1` | `0xf2c12d5b` | -222220965 | confirmed spelling | PS2 + GC |
