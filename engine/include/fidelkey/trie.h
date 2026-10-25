#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>

namespace fidelkey {
namespace engine {

class TrieNode {
public:
    TrieNode();
    std::map<wchar_t, std::unique_ptr<TrieNode>>& GetChildren();
    bool HasChildren() const;
    void SetValue(const std::wstring& value);
    const std::wstring& GetValue() const;
    bool HasValue() const;
    
private:
    std::map<wchar_t, std::unique_ptr<TrieNode>> children;
    std::wstring value;
    bool hasValue = false;
};

class Trie {
public:
    Trie();
    void Insert(const std::wstring& key, const std::wstring& value);
    // Find longest match starting from a string
    std::pair<std::wstring, size_t> FindLongestMatch(const std::wstring& input) const;
    
private:
    std::unique_ptr<TrieNode> root;
};

} // namespace engine
} // namespace fidelkey
