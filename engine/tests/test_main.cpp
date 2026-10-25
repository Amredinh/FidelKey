#include <iostream>
#include <cassert>
#include <fidelkey/engine.h>
#include <codecvt>
#include <locale>
#include <string>
#include <vector>

using namespace fidelkey::engine;

std::string wstring_to_utf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

void test_selam() {
    std::cout << "\n=== Test: selam -> ሰላም (REQUIRED) ===" << std::endl;
    PhoneticEngine engine;
    
    // s
    auto r1 = engine.ProcessKey(L's');
    std::cout << "s: consumed=" << r1.consumed << ", comp='" << wstring_to_utf8(r1.composition) << "'" << std::endl;
    assert(r1.consumed);
    
    // e
    auto r2 = engine.ProcessKey(L'e');
    std::cout << "se: consumed=" << r2.consumed << ", comp='" << wstring_to_utf8(r2.composition) << "'" << std::endl;
    assert(r2.consumed);
    
    // l
    auto r3 = engine.ProcessKey(L'l');
    std::cout << "sel: consumed=" << r3.consumed << ", comp='" << wstring_to_utf8(r3.composition) << "'" << std::endl;
    assert(r3.consumed);
    
    // a
    auto r4 = engine.ProcessKey(L'a');
    std::cout << "sela: consumed=" << r4.consumed << ", comp='" << wstring_to_utf8(r4.composition) << "'" << std::endl;
    assert(r4.consumed);
    
    // m
    auto r5 = engine.ProcessKey(L'm');
    std::cout << "selam: consumed=" << r5.consumed << ", comp='" << wstring_to_utf8(r5.composition) << "'" << std::endl;
    assert(r5.consumed);
    
    // space - commit
    auto r6 = engine.ProcessKey(L' ');
    std::cout << "space: output='" << wstring_to_utf8(r6.output) << "', shouldCommit=" << r6.shouldCommit << std::endl;
    assert(r6.shouldCommit);
    // Expected: ሰ + ላ + ም = ሰላም
    std::wstring expected = L"ሰላም ";
    assert(r6.output == expected || r6.output.find(L"ሰላም") != std::wstring::npos);
    std::cout << "PASSED: selam test" << std::endl;
}

void test_basic_mappings() {
    std::cout << "\n=== Test: Basic mappings ===" << std::endl;
    PhoneticEngine engine;
    
    // Test 'a' alone
    engine.Reset();
    auto r = engine.ProcessKey(L'a');
    assert(r.consumed);
    std::cout << "'a': comp='" << wstring_to_utf8(r.composition) << "'" << std::endl;
    auto rs = engine.ProcessKey(L' ');
    std::cout << "'a ': out='" << wstring_to_utf8(rs.output) << "'" << std::endl;
    assert(rs.output.find(L"አ") != std::wstring::npos);
    
    // Test 'm' alone
    engine.Reset();
    r = engine.ProcessKey(L'm');
    std::cout << "'m': comp='" << wstring_to_utf8(r.composition) << "'" << std::endl;
    rs = engine.ProcessKey(L' ');
    std::cout << "'m ': out='" << wstring_to_utf8(rs.output) << "'" << std::endl;
    
    std::cout << "PASSED: basic mappings" << std::endl;
}

void test_backspace() {
    std::cout << "\n=== Test: Backspace ===" << std::endl;
    PhoneticEngine engine;
    
    engine.ProcessKey(L's');
    engine.ProcessKey(L'e');
    std::cout << "after se: comp='" << wstring_to_utf8(engine.GetComposition()) << "'" << std::endl;
    
    auto bs = engine.ProcessBackspace();
    std::cout << "after backspace: comp='" << wstring_to_utf8(bs.composition) << "'" << std::endl;
    assert(bs.consumed);
    
    std::cout << "PASSED: backspace" << std::endl;
}

void test_mixed_english() {
    std::cout << "\n=== Test: Mixed English ===" << std::endl;
    PhoneticEngine engine;
    
    engine.Reset();
    auto r1 = engine.ProcessKey(L'h');
    auto r2 = engine.ProcessKey(L'e');
    auto r3 = engine.ProcessKey(L'l');
    auto r4 = engine.ProcessKey(L'l');
    auto r5 = engine.ProcessKey(L'o');
    std::cout << "'hello': comp='" << wstring_to_utf8(engine.GetComposition()) << "'" << std::endl;
    auto rs = engine.ProcessKey(L' ');
    std::cout << "commit: out='" << wstring_to_utf8(rs.output) << "'" << std::endl;
    
    // Should pass through as English
    assert(rs.output.find(L"hello") != std::wstring::npos);
    
    std::cout << "PASSED: mixed english" << std::endl;
}

void test_punctuation() {
    std::cout << "\n=== Test: Punctuation ===" << std::endl;
    PhoneticEngine engine;
    
    engine.Reset();
    engine.ProcessKey(L's');
    engine.ProcessKey(L'e');
    auto rp = engine.ProcessKey(L'.');
    std::cout << "'se.': out='" << wstring_to_utf8(rp.output) << "'" << std::endl;
    assert(rp.shouldCommit);
    
    std::cout << "PASSED: punctuation" << std::endl;
}

void test_overlapping_prefixes() {
    std::cout << "\n=== Test: Overlapping prefixes ===" << std::endl;
    PhoneticEngine engine;
    
    // Test cases where one sequence is prefix of another
    // e.g., 's' could be start of 'se', 'sh', etc.
    engine.Reset();
    engine.ProcessKey(L's');
    std::cout << "just 's': comp='" << wstring_to_utf8(engine.GetComposition()) << "'" << std::endl;
    engine.ProcessKey(L'h');
    std::cout << "'sh': comp='" << wstring_to_utf8(engine.GetComposition()) << "'" << std::endl;
    auto rs = engine.ProcessKey(L' ');
    std::cout << "'sh ': out='" << wstring_to_utf8(rs.output) << "'" << std::endl;
    
    std::cout << "PASSED: overlapping prefixes" << std::endl;
}

int main() {
    std::cout << "FidelKey Engine Tests - Milestone 1 Validation" << std::endl;
    std::cout << "=================================================" << std::endl;
    
    try {
        test_selam();
        test_basic_mappings();
        test_backspace();
        test_mixed_english();
        test_punctuation();
        test_overlapping_prefixes();
        std::cout << "\n=== ALL TESTS PASSED ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nFAILED: " << e.what() << std::endl;
        return 1;
    }
}
