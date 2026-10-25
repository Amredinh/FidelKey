#include "fidelkey/trie.h"

namespace fidelkey {
namespace engine {

TrieNode::TrieNode() = default;

std::map<wchar_t, std::unique_ptr<TrieNode>>& TrieNode::GetChildren() {
    return children;
}

bool TrieNode::HasChildren() const {
    return !children.empty();
}

void TrieNode::SetValue(const std::wstring& val) {
    value = val;
    hasValue = true;
}

const std::wstring& TrieNode::GetValue() const {
    return value;
}

bool TrieNode::HasValue() const {
    return hasValue;
}

Trie::Trie() {
    root = std::make_unique<TrieNode>();
}

void Trie::Insert(const std::wstring& key, const std::wstring& val) {
    TrieNode* current = root.get();
    for (wchar_t ch : key) {
        if (current->GetChildren().find(ch) == current->GetChildren().end()) {
            current->GetChildren()[ch] = std::make_unique<TrieNode>();
        }
        current = current->GetChildren()[ch].get();
    }
    current->SetValue(val);
}

std::pair<std::wstring, size_t> Trie::FindLongestMatch(const std::wstring& input) const {
    TrieNode* current = root.get();
    TrieNode* lastMatchNode = nullptr;
    size_t lastMatchLength = 0;
    
    for (size_t i = 0; i < input.length(); ++i) {
        wchar_t ch = input[i];
        auto it = current->GetChildren().find(ch);
        if (it == current->GetChildren().end()) {
            break;
        }
        current = it->second.get();
        if (current->HasValue()) {
            lastMatchNode = current;
            lastMatchLength = i + 1;
        }
    }
    
    if (lastMatchNode && lastMatchNode->HasValue()) {
        return {lastMatchNode->GetValue(), lastMatchLength};
    }
    
    return {L"", 0};
}

} // namespace engine
} // namespace fidelkey
