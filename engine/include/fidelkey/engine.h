#pragma once

#include <string>
#include <memory>
#include <vector>

namespace fidelkey {
namespace engine {

struct ConversionResult {
    bool consumed = false;
    bool shouldCommit = false;
    std::wstring output;
    std::wstring composition;
};

struct EngineConfig {
    std::string layoutName = "gff_phonetic";
    bool enableBackspace = true;
};

class PhoneticEngine {
public:
    PhoneticEngine();
    explicit PhoneticEngine(const EngineConfig& config);
    ~PhoneticEngine();
    
    PhoneticEngine(const PhoneticEngine&) = delete;
    PhoneticEngine& operator=(const PhoneticEngine&) = delete;
    PhoneticEngine(PhoneticEngine&&) = default;
    PhoneticEngine& operator=(PhoneticEngine&&) = default;
    
    ConversionResult ProcessKey(wchar_t key);
    ConversionResult ProcessBackspace();
    void Reset();
    std::wstring GetComposition() const;
    bool HasComposition() const;
    ConversionResult Commit();
    void SetLayout(const std::string& layoutName);
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace engine
} // namespace fidelkey
