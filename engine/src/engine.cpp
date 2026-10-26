#include "fidelkey/engine.h"
#include "fidelkey/trie.h"
#include "fidelkey/mapping.h"
#include <string>
#include <vector>
#include <cwctype>

namespace fidelkey {
namespace engine {

// The engine keeps the raw Latin keystrokes for the whole word and derives the
// Ethiopic display greedily from the left. This means nothing is committed
// mid-word, so backspace always operates on the word the user is typing.
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

    // Convert as much of `buffer` as possible, left to right, taking the
    // longest mapping at each position. Unmapped characters pass through.
    std::wstring Convert(const std::wstring& buffer) const {
        std::wstring out;
        size_t i = 0;
        while (i < buffer.size()) {
            auto match = trie->FindLongestMatch(buffer.substr(i));
            if (!match.first.empty() && match.second > 0) {
                out += match.first;
                i += match.second;
            } else {
                out += buffer[i];
                ++i;
            }
        }
        return out;
    }

    ConversionResult ProcessKey(wchar_t key) {
        ConversionResult result;

        auto flushWith = [&](wchar_t boundary) {
            result.output = Convert(latinBuffer);
            latinBuffer.clear();
            composedText.clear();
            result.output += boundary;
            result.shouldCommit = true;
            result.consumed = true;
            result.composition.clear();
            return result;
        };

        // Space, punctuation and digits end the current word and pass through.
        if (iswspace(key) || iswpunct(key) || iswdigit(key)) {
            return flushWith(key);
        }

        // Letters are always composed. Unmapped letters stay as literals.
        if (iswalpha(key)) {
            latinBuffer += key;
            composedText = Convert(latinBuffer);
            result.consumed = true;
            result.shouldCommit = false;
            result.composition = composedText;
            return result;
        }

        // Other keys end the word but are not consumed by us.
        result.output = Convert(latinBuffer);
        latinBuffer.clear();
        composedText.clear();
        result.shouldCommit = !result.output.empty();
        result.consumed = false;
        return result;
    }

    ConversionResult ProcessBackspace() {
        ConversionResult result;
        if (latinBuffer.empty()) {
            result.consumed = false;
            return result;
        }

        latinBuffer.pop_back();
        composedText = Convert(latinBuffer);
        result.consumed = true;
        result.shouldCommit = false;
        result.composition = composedText;
        return result;
    }

    void Reset() {
        latinBuffer.clear();
        composedText.clear();
    }

    std::wstring GetComposition() const {
        return composedText;
    }

    bool HasComposition() const {
        return !latinBuffer.empty();
    }

    ConversionResult Commit() {
        ConversionResult result;
        if (!latinBuffer.empty()) {
            result.output = Convert(latinBuffer);
            result.shouldCommit = true;
        }
        latinBuffer.clear();
        composedText.clear();
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
    std::wstring latinBuffer;      // Raw Latin keystrokes for the current word
    std::wstring composedText;     // Current best Ethiopic rendering
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
