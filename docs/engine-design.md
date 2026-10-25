# Conversion Engine Design

## Overview
Standalone C++ library for Latin→Ethiopic conversion, independent of TSF/Windows for testability.

## API
```cpp
namespace fidelkey {
namespace engine {
struct ConversionResult {
    bool consumed = false;      // Key consumed by engine
    bool shouldCommit = false;  // Should commit output
    std::wstring output;        // Text to commit
    std::wstring composition;   // Preview text
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

## Behavior Rules (Defined)

### Character Buffering
- **Latin letters (a-z, A-Z)**: Added to `latinBuffer`, consumed by engine
- **Non-letters (space, punctuation, digits)**: Commit any pending composition in buffer, pass through the character
- **Other chars**: Commit pending composition, pass through

### Preview (Composition)
- When buffer forms a valid prefix or complete match, `composition` shows the Ethiopic equivalent
- During partial input, shows current composition state
- Updated on each keystroke and backspace

### Commit
- On space, punctuation, digit, or other non-letter: pending composition is committed
- On explicit `Commit()` call
- `shouldCommit=true` indicates text in `output` should be committed to application

### Pass-through
- English text with no Amharic mapping: accumulated in buffer, then committed as-is when boundary hit
- Digits, punctuation always pass through
- After committing, engine resets state

## Trie Matching
- Uses trie for prefix matching
- Finds longest match in buffer
- Greedy matching
- Deterministic - no timeouts
- If sequence is both complete match and prefix of longer: current logic tries longest match; may need refinement based on testing

## Backspace
- Removes last char from latinBuffer
- Re-matches to update composition
- If buffer becomes empty, composition cleared
- Consumed by engine when there's composition to delete

## Overlapping Prefixes
Example: 's' vs 'sh' vs 'se' - all start differently
- 's' alone: no match yet? Or matches 's' → ስ (6th order)
- Need to check if 's' is a complete mapping

Looking at mapping: we have `AddMapping(L"s", L"ስ")` - so 's' alone maps to ስ
Also `AddMapping(L"se", L"ሰ")`, `AddMapping(L"sh", L"ሽ")`

Trie will find longest match. For input "se": trie finds "se" matches ሰ (length 2). Correct.
For input "s ": when space comes, we commit based on buffer "s" which matches ስ.

## Potential Issues
1. **Ambiguity**: If user wants to type English 's' followed by space in Amharic mode - becomes ስ. This is expected in phonetic mode.
2. **Timing**: No way to force commit of partial composition except via boundary/Commit()
3. **API limitation**: TSF needs to know whether to update composition or commit - current design should work

## Testing Status
Tests written but not executed locally (per constraints). Will be validated in CI.

## Future Considerations
- May need to handle case where user wants to type Latin letters without conversion
- Could add mode switching
- Could improve disambiguation rules
