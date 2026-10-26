# FidelKey Mapping Specification

**Version:** 2.0
**Reference:** Ge'ez Frontier Foundation (GFF) Amharic keyboard, Keyman `gff_amh_7` / `gff_amharic` 3.3
**Implementation:** `engine/src/mapping.cpp`

## Scope

FidelKey implements one deterministic phonetic layout. The traditional Fidel
physical-key layout is **not implemented yet** and is explicitly marked as
planned in the settings app.

The engine is self-contained. The TSF service does not contain any conversion
rules; it only calls the engine.

## Vowel pattern

A consonant family is typed as a base key followed by a vowel suffix.

| Suffix | Order | `s` example |
|--------|-------|-------------|
| `e` | 1st | ሰ |
| `u` | 2nd | ሱ |
| `i` | 3rd | ሲ |
| `a` | 4th | ሳ |
| `ie` (alias `ee`) | 5th | ሴ |
| none | 6th | ስ |
| `o` | 7th | ሶ |

GFF's 6th order is the bare consonant key. This is why `s` alone produces ስ,
while `se` produces ሰ.

## Standalone vowels (glottal family)

| Keys | Character |
|------|-----------|
| `a` | አ |
| `u` | ኡ |
| `i` | ኢ |
| `aa` | ኣ |
| `ie` / `ee` | ኤ |
| `e` | እ |
| `o` | ኦ |

## Consonant families

| Base | 1st | 2nd | 3rd | 4th | 5th | 6th | 7th |
|------|-----|-----|-----|-----|-----|-----|-----|
| `h` | ሀ | ሁ | ሂ | ሃ | ሄ | ህ | ሆ |
| `l` | ለ | ሉ | ሊ | ላ | ሌ | ል | ሎ |
| `H` | ሐ | ሑ | ሒ | ሓ | ሔ | ሕ | ሖ |
| `m` | መ | ሙ | ሚ | ማ | ሜ | ም | ሞ |
| `ss` | ሠ | ሡ | ሢ | ሣ | ሤ | ሥ | ሦ |
| `r` | ረ | ሩ | ሪ | ራ | ሬ | ር | ሮ |
| `s` | ሰ | ሱ | ሲ | ሳ | ሴ | ስ | ሶ |
| `x` (`sh`) | ሸ | ሹ | ሺ | ሻ | ሼ | ሽ | ሾ |
| `q` | ቀ | ቁ | ቂ | ቃ | ቄ | ቅ | ቆ |
| `b` | በ | ቡ | ቢ | ባ | ቤ | ብ | ቦ |
| `v` | ቨ | ቩ | ቪ | ቫ | ቬ | ቭ | ቮ |
| `t` | ተ | ቱ | ቲ | ታ | ቴ | ት | ቶ |
| `c` (`ch`) | ቸ | ቹ | ቺ | ቻ | ቼ | ች | ቾ |
| `hh` | ኀ | ኁ | ኂ | ኃ | ኄ | ኅ | ኆ |
| `n` | ነ | ኑ | ኒ | ና | ኔ | ን | ኖ |
| `N` | ኘ | ኙ | ኚ | ኛ | ኜ | ኝ | ኞ |
| `k` | ከ | ኩ | ኪ | ካ | ኬ | ክ | ኮ |
| `K` | ኸ | ኹ | ኺ | ኻ | ኼ | ኽ | ኾ |
| `w` | ወ | ዉ | ዊ | ዋ | ዌ | ው | ዎ |
| `z` | ዘ | ዙ | ዚ | ዛ | ዜ | ዝ | ዞ |
| `Z` | ዠ | ዡ | ዢ | ዣ | ዤ | ዥ | ዦ |
| `y` | የ | ዩ | ዪ | ያ | ዬ | ይ | ዮ |
| `d` | ደ | ዱ | ዲ | ዳ | ዴ | ድ | ዶ |
| `j` | ጀ | ጁ | ጂ | ጃ | ጄ | ጅ | ጆ |
| `g` | ገ | ጉ | ጊ | ጋ | ጌ | ግ | ጎ |
| `T` | ጠ | ጡ | ጢ | ጣ | ጤ | ጥ | ጦ |
| `C` | ጨ | ጩ | ጪ | ጫ | ጬ | ጭ | ጮ |
| `P` | ጰ | ጱ | ጲ | ጳ | ጴ | ጵ | ጶ |
| `S` | ጸ | ጹ | ጺ | ጻ | ጼ | ጽ | ጾ |
| `SS` | ፀ | ፁ | ፂ | ፃ | ፄ | ፅ | ፆ |
| `f` | ፈ | ፉ | ፊ | ፋ | ፌ | ፍ | ፎ |
| `p` | ፐ | ፑ | ፒ | ፓ | ፔ | ፕ | ፖ |

## Labiovelars

Labiovelar forms insert `u` before the vowel: `ue`, `uu`, `ui`, `ua`, `uie`.

| Family | Forms |
|--------|-------|
| `q` | `que` ቈ · `quu` ቍ · `qui` ቊ · `qua` ቋ · `quie` ቌ |
| `hh` | `hhue` ኈ · `hhuu` ኍ · `hhui` ኊ · `hhua` ኋ · `hhuie` ኌ |
| `k` | `kue` ኰ · `kuu` ኵ · `kui` ኲ · `kua` ኳ · `kuie` ኴ |
| `K` | `Kue` ዀ · `Kuu` ዅ · `Kui` ዂ · `Kua` ዃ · `Kuie` ዄ |
| `g` | `gue` ጐ · `guu` ጕ · `gui` ጒ · `gua` ጓ · `guie` ጔ |

Families that have labiovelar letters in Unicode but that GFF reaches through
other conventions (`l`, `H`, `m`, `ss`, `r`, `s`, `x`, `b`, `v`, `t`, `c`, `n`,
`z`, `d`, `j`, `T`, `C`, `P`, `S`, `f`, `p`) accept the `ue`/`ua` forms.

## Convenience aliases

These are additive and do not change GFF-native keys:

- `sh` behaves exactly like `x` (ሸ family).
- `ch` behaves exactly like `c` (ቸ family).
- `ee` behaves exactly like `ie` (5th order, and standalone ኤ).

## Unsupported by design

- The ዐ (pharyngeal) family and non-Amharic families (ጘ, ⶠ, …) are not exposed.
- Uppercase letters outside the families above are passed through literally.
- `e` is the 6th-order glottal (እ), never a consonant 1st order on its own.

## Engine behavior

1. Latin letters `a`–`z` and `A`–`Z` are buffered for the current word.
2. The whole word is converted greedily from left to right using the longest
   mapping at each position.
3. Unmapped letters remain literal, so mixed text is preserved.
4. Space, punctuation and digits end the word: the converted word is committed
   and the boundary character is returned unchanged.
5. Backspace removes the last Latin keystroke and recomputes the whole word, so
   backspace always edits what the user just typed.
6. Nothing is committed mid-word. A composition can be cancelled by clearing all
   keystrokes with backspace.

### Acceptance tests

| Input | Output |
|-------|--------|
| `selam` | ሰላም |
| `amen` | አመን |
| `abeba` | አበባ |
| `S` | ጽ |
| `SS` | ፅ |
| `Kua` | ዃ |
| `hhua` | ኋ |
| `gua` | ጓ |

The suite in `engine/tests/test_main.cpp` checks every table entry, every family
and order, every labiovelar, aliases, backspace, composition state, duplicate
keys, and boundary behavior. It returns a non-zero exit code on any failure and
is not compiled out in Release builds.

## Known limitations

- The traditional physical-key layout is not implemented.
- No transliteration disambiguation UI or suggestions.
- Only the mappings above are recognized; unknown sequences pass through.
