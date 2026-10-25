#include "fidelkey/engine.h"
#include "fidelkey/trie.h"
#include "fidelkey/mapping.h"
#include <string>
#include <vector>
#include <cwctype>

namespace fidelkey {
namespace engine {

class PhoneticEngine::Impl {
public:
    Impl() {
        mappingTable.LoadGFFPhonetic();
        BuildTrie();
    }
    
    explicit Impl(const EngineConfig& cfg) : config(cfg) {
        mappingTable.LoadGFFPhonetic();
        BuildTrie();
    }
    
    void BuildTrie() {
        trie = std::make_unique<Trie>();
        for (const auto& entry : mappingTable.GetAllMappings()) {
            trie->Insert(entry.latin, entry.ethiopic);
        }
    }
    
    ConversionResult ProcessKey(wchar_t key) {
        ConversionResult result;
        
        // Handle space, punctuation, digits - commit any pending composition
        if (iswspace(key) || iswpunct(key) || iswdigit(key)) {
            if (!latinBuffer.empty()) {
                // Try to commit any complete match in buffer
                auto match = trie->FindLongestMatch(latinBuffer);
                if (!match.first.empty() && match.second == latinBuffer.length()) {
                    result.output = match.first;
                } else if (!match.first.empty() && match.second > 0) {
                    // Partial match - commit what matches, keep remainder? Or just commit match
                    result.output = match.first;
                    // Remainder would be unconverted - but for now, commit match
                } else {
                    // No match, output as-is
                    result.output = latinBuffer;
                }
                result.shouldCommit = true;
                latinBuffer.clear();
                composedText.clear();
            }
            // Pass through the key
            result.output += key;
            result.consumed = true;
            result.shouldCommit = true;
            return result;
        }
        
        // Handle letters
        if (iswalpha(key)) {
            // Add to latin buffer
            latinBuffer += key;
            
            // Try to find longest match for the entire buffer
            auto match = trie->FindLongestMatch(latinBuffer);
            if (!match.first.empty()) {
                if (match.second == latinBuffer.length()) {
                    // Exact match of entire buffer
                    composedText = match.first;
                    result.consumed = true;
                    result.composition = composedText;
                    // Don't commit yet - could be extended? Check if there might be longer matches
                    // For now, show composition; will commit on space/punctuation
                    return result;
                } else if (match.second < latinBuffer.length()) {
                    // We have a match for a prefix but buffer is longer
                    // This means we need to commit the matched part and start fresh
                    result.output = match.first;
                    result.shouldCommit = true;
                    // Keep the remainder
                    std::wstring remainder = latinBuffer.substr(match.second);
                    latinBuffer = remainder;
                    // Check remainder
                    auto match2 = trie->FindLongestMatch(latinBuffer);
                    if (!match2.first.empty() && match2.second == latinBuffer.length()) {
                        composedText = match2.first;
                        result.consumed = true;
                        result.composition = composedText;
                    } else {
                        composedText = latinBuffer;
                        result.consumed = true;
                        result.composition = composedText;
                    }
                    return result;
                } else {
                    // match.second > latinBuffer.length() shouldn't happen
                    result.consumed = true;
                    result.composition = match.first;
                    return result;
                }
            } else {
                // No match found
                // Check if adding this char broke all possible matches
                // If buffer has content but no match, it might be invalid sequence
                // For English letters that don't form Amharic, pass them through?
                // But we need to be careful not to lose input
                result.consumed = true;
                result.composition = latinBuffer;
                return result;
            }
        }
        
        // Other keys pass through
        if (!latinBuffer.empty()) {
            auto match = trie->FindLongestMatch(latinBuffer);
            if (!match.first.empty() && match.second == latinBuffer.length()) {
                result.output = match.first;
            } else if (!match.first.empty()) {
                result.output = match.first;
            } else {
                result.output = latinBuffer;
            }
            result.shouldCommit = true;
            latinBuffer.clear();
            composedText.clear();
        }
        result.output += key;
        result.consumed = false;
        result.shouldCommit = true;
        return result;
    }
    
    ConversionResult ProcessBackspace() {
        ConversionResult result;
        if (!latinBuffer.empty()) {
            if (latinBuffer.length() == 1) {
                latinBuffer.clear();
                composedText.clear();
                result.consumed = true;
                result.composition.clear();
                return result;
            } else {
                // Remove last character
                latinBuffer.pop_back();
                // Re-match
                auto match = trie->FindLongestMatch(latinBuffer);
                if (!match.first.empty() && match.second == latinBuffer.length()) {
                    composedText = match.first;
                    result.composition = composedText;
                } else if (!match.first.empty()) {
                    composedText = match.first;
                    result.composition = composedText;
                } else {
                    composedText = latinBuffer;
                    result.composition = composedText;
                }
                result.consumed = true;
                return result;
            }
        }
        result.consumed = false;
        return result;
    }
    
    void Reset() {
        latinBuffer.clear();
        composedText.clear();
    }
    
    std::wstring GetComposition() const {
        if (!composedText.empty()) {
            return composedText;
        }
        return latinBuffer;
    }
    
    bool HasComposition() const {
        return !latinBuffer.empty();
    }
    
    ConversionResult Commit() {
        ConversionResult result;
        if (!latinBuffer.empty()) {
            auto match = trie->FindLongestMatch(latinBuffer);
            if (!match.first.empty() && match.second == latinBuffer.length()) {
                result.output = match.first;
            } else if (!match.first.empty()) {
                result.output = match.first;
            } else {
                result.output = latinBuffer;
            }
            result.shouldCommit = true;
            latinBuffer.clear();
            composedText.clear();
        }
        return result;
    }
    
    void SetLayout(const std::string& layoutName) {
        config.layoutName = layoutName;
        BuildTrie();
    }
    
private:
    EngineConfig config;
    MappingTable mappingTable;
    std::unique_ptr<Trie> trie;
    std::wstring latinBuffer;      // Buffer of latin keystrokes
    std::wstring composedText;     // Currently composed Ethiopic text
};

PhoneticEngine::PhoneticEngine() : pImpl(std::make_unique<Impl>()) {}
PhoneticEngine::PhoneticEngine(const EngineConfig& config) : pImpl(std::make_unique<Impl>(config)) {}
PhoneticEngine::~PhoneticEngine() = default;

ConversionResult PhoneticEngine::ProcessKey(wchar_t key) {
    return pImpl->ProcessKey(key);
}

ConversionResult PhoneticEngine::ProcessBackspace() {
    return pImpl->ProcessBackspace();
}

void PhoneticEngine::Reset() {
    pImpl->Reset();
}

std::wstring PhoneticEngine::GetComposition() const {
    return pImpl->GetComposition();
}

bool PhoneticEngine::HasComposition() const {
    return pImpl->HasComposition();
}

ConversionResult PhoneticEngine::Commit() {
    return pImpl->Commit();
}

void PhoneticEngine::SetLayout(const std::string& layoutName) {
    pImpl->SetLayout(layoutName);
}

} // namespace engine
} // namespace fidelkey
