#pragma once

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

namespace fidelkey {
namespace engine {

// Mapping entry
struct MappingEntry {
    std::wstring latin;
    std::wstring ethiopic;
    int priority = 0; // for disambiguation
};

// Mapping table
class MappingTable {
public:
    MappingTable();
    void LoadGFFPhonetic();
    const std::vector<MappingEntry>& GetAllMappings() const;
    void AddMapping(const std::wstring& latin, const std::wstring& ethiopic, int priority = 0);
    
private:
    std::vector<MappingEntry> mappings;
};

} // namespace engine
} // namespace fidelkey
