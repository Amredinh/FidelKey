# FidelKey - Mapping Specification

**Version:** 1.1  
**Date:** 2026-10-24  
**Reference:** Ge'ez Frontier Foundation (GFF) Amharic Layout (Keyman gff_amh_7)  
**Status:** Under Validation - Milestone 1  

## Purpose
This document defines the phonetic mapping for FidelKey. Based on GFF conventions where practical, with documented differences.

## Verification Status

### Verified Mappings (tested logic)
- Basic vowel forms with common consonants
- Simple syllables (consonant + vowel)
- Space/punctuation handling
- Backspace behavior (basic)

### Intentionally Different
- Some implementation details for composition vs commit timing
- Engine buffers until word boundary (space/punctuation) for easier correction

### Uncertain - Need Testing
- **ሀ/ሐ/ኀ families**: Three different families in Ethiopic. GFF distinguishes them. Need to confirm correct keys.
  - ሀ family (ha, hu, hi, ha, hie, h, ho) - uses 'h' 
  - ሐ family (HHa, HHu...) - uses capital 'H' or 'hh' per GFF
  - ኀ family (xa, xu...) - uses 'x' per GFF
- **ሰ vs ሠ**: ሰ uses 's', ሠ uses capital 'S' or 'ss' per some conventions
- **ጸ vs ፀ**: Different families, need correct mapping
- **ቸ vs ጨ**: 'ch'/'c' vs capital 'C'/'cc'
- **ተ vs ጠ**: 't' vs capital 'T'/'tt'
- **ከ vs ኸ**: 'k' vs capital 'K'/'kk'
- **Labiovelar sequences** (kwa, gwa, qwa, etc.): Need validation of all forms
- **ላዊ forms** (5th order labiovelars like ሏ): Adding 'ua'/'wa'

## Core Vowel Patterns (GFF Standard)
| Input | Order | Example with 's' |
|-------|-------|-------------------|
| e | 1st/6th | ሰ, ስ |
| u | 2nd | ሱ |
| i | 3rd | ሲ |
| a | 4th | ሳ |
| ie/ee | 5th | ሴ |
| (just consonant) | 6th | ስ |
| o | 7th | ሶ |

## Implemented Mappings (see mapping.cpp for full list)
All mappings from engine/src/mapping.cpp - needs validation against actual GFF spec.

## Testing Coverage
- selam → ሰላም (required test) - implemented
- Basic mappings - implemented  
- Backspace - implemented
- Mixed English - implemented
- Punctuation - implemented
- Overlapping prefixes - implemented

**Note:** Tests are written but not executed locally per constraints. Must run in CI.

## Known Issues
1. Composition/commit logic may need refinement for some cases
2. English passthrough behavior needs validation
3. Some ambiguous cases need clearer rules

## References
- [GFF Amharic Typing Manual](https://help.keyman.com/keyboard/gff_amh_7/1.6/AmharicTyping-English.pdf)
- [Keyman GFF Amharic](https://keyman.com/keyboards/gff_amharic)
