# Conversion Engine Design

## Overview

A small, dependency-free C++ library that converts Latin keystrokes into
Ethiopic text. It has no Windows or TSF dependencies, so it can be tested on any
platform and is shared by the TSF service by linking `fidelkey_engine`.

Files:

- `engine/include/fidelkey/engine.h` — public API
- `engine/include/fidelkey/mapping.h` — mapping table
- `engine/include/fidelkey/trie.h` — prefix trie
- `engine/src/engine.cpp` — conversion and composition state
- `engine/src/mapping.cpp` — GFF 3.3 mapping data
- `engine/tests/test_main.cpp` — test suite

## API

```cpp
namespace fidelkey {
namespace engine {

struct ConversionResult {
    bool consumed = false;      // engine handled the key
    bool shouldCommit = false;  // output must be written to the document
    std::wstring output;        // text to commit
    std::wstring composition;   // current preview
};

class PhoneticEngine {
public:
    ConversionResult ProcessKey(wchar_t key);
    ConversionResult ProcessBackspace();
    void Reset();
    std::wstring GetComposition() const;
    bool HasComposition() const;
    ConversionResult Commit();
    void SetLayout(const std::string& layoutName);
};

}
}
```

## Conversion algorithm

The engine keeps the raw Latin keystrokes for the **whole current word** and
renders them greedily:

```
Convert(buffer):
    i = 0
    while i < len(buffer):
        match = trie.FindLongestMatch(buffer[i:])
        if match.length > 0:
            append match.text; i += match.length
        else:
            append buffer[i]; i += 1
```

Consequences:

- No partial match is ever committed mid-word, so every keystroke can still be
  edited with backspace.
- `selam` → `ሰ` + `ላ` + `ም` = ሰላም.
- Unmapped characters are preserved verbatim, so `ABC` stays `ABC`.
- The trie gives deterministic, O(n·depth) behavior with no timers or async
  state.

## Composition state

| Event | Result |
|-------|--------|
| Latin letter | `consumed = true`, buffer extended, `composition` updated |
| Space / punctuation / digit | converted word returned in `output`, boundary char appended, buffer cleared |
| Unhandled key | converted word returned (if any), key not consumed |
| Backspace | last Latin key removed, `composition` recomputed; not consumed when buffer is empty |
| `Commit()` | returns converted buffer and clears it |

`GetComposition()` returns the current Ethiopic preview. `HasComposition()` is
`true` while the Latin buffer is non-empty.

### Backspace rules

- While composing, backspace edits the Latin buffer, not the Ethiopic preview.
- Deleting the last character of the buffer ends the composition without
  inserting anything.
- Once a word has been committed at a boundary, the buffer is empty and
  backspace is passed to the application normally.

## Trie

`Trie::Insert` stores a value at each key node. `Trie::FindLongestMatch` walks
the input and remembers the deepest node with a value. Duplicate keys overwrite
the earlier value; the test suite fails if any duplicate key exists so this is
never silent.

## TSF integration

`CFidelKeyTextService` owns one `PhoneticEngine` per input thread:

- Letters are routed through `OnKeyDown`.
- The current `GetComposition()` string is written into an `ITfComposition`
  range via an `ITfEditSession`.
- Backspace updates or ends the composition.
- Any other key (space, punctuation, Enter, arrows, …) finalizes the composition
  synchronously in `OnTestKeyDown` and is then passed to the application, so
  boundary characters are inserted by the app rather than synthesized.
- `OnCompositionTerminated` releases the composition and resets the engine.

All edits use `TF_ES_SYNC | TF_ES_READWRITE`, which keeps engine state and
document state ordered.

## Testing

`engine/tests/test_main.cpp` uses a small `CHECK` harness rather than `assert`,
so failures are reported and the process exits non-zero in Release builds. It
checks:

- whole-word outputs (`selam`, `amen`, `abeba`, boundaries, mixed text)
- composition and no-premature-commit behavior
- backspace through compositions
- every family and vowel order
- every labiovelar form
- aliases (`sh`, `ch`, `ee`)
- every table entry round-trips through the engine
- duplicate mapping keys are rejected

## Limitations

- Traditional Fidel physical-key layout is not implemented.
- No disambiguation/suggestion layer; the longest match wins.
- Layout selection is stored by the settings app but only the phonetic layout is
  available in this version.
