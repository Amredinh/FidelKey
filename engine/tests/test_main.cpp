#include <fidelkey/engine.h>
#include <fidelkey/mapping.h>

#include <codecvt>
#include <iostream>
#include <locale>
#include <set>
#include <string>
#include <vector>

using namespace fidelkey::engine;

// ---------------------------------------------------------------------------
// Release-safe test harness.
// The previous suite used assert(), which is compiled out under NDEBUG, so a
// Release build "passed" without checking anything. This harness always runs
// and returns a non-zero exit code on any failure.
// ---------------------------------------------------------------------------

static int g_checks = 0;
static int g_failures = 0;

static std::string U8(const std::wstring& s) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(s);
}

static std::string U8(const wchar_t* s) {
    return U8(std::wstring(s));
}

static void CheckEq(const std::wstring& actual, const std::wstring& expected,
                    const std::string& what, int line) {
    ++g_checks;
    if (actual != expected) {
        ++g_failures;
        std::cerr << "FAIL line " << line << ": " << what
                  << " -> got [" << U8(actual) << "] expected [" << U8(expected) << "]\n";
    }
}

static void CheckTrue(bool cond, const std::string& what, int line) {
    ++g_checks;
    if (!cond) {
        ++g_failures;
        std::cerr << "FAIL line " << line << ": " << what << "\n";
    }
}

#define CHECK_EQ(actual, expected) CheckEq((actual), (expected), #actual " == " #expected, __LINE__)
#define CHECK_TRUE(cond) CheckTrue((cond), #cond, __LINE__)

// Type every character through the engine, committing on boundaries, then flush.
static std::wstring Type(PhoneticEngine& engine, const std::wstring& keys) {
    std::wstring out;
    for (wchar_t k : keys) {
        ConversionResult r = engine.ProcessKey(k);
        if (r.shouldCommit) {
            out += r.output;
        }
    }
    ConversionResult tail = engine.Commit();
    out += tail.output;
    return out;
}

static std::wstring Type(const std::wstring& keys) {
    PhoneticEngine engine;
    return Type(engine, keys);
}

// ---------------------------------------------------------------------------
// Whole-word behavior
// ---------------------------------------------------------------------------
static void TestWords() {
    CHECK_EQ(Type(L"selam"), std::wstring(L"ሰላም"));
    CHECK_EQ(Type(L"amen"), std::wstring(L"አመን"));
    CHECK_EQ(Type(L"abeba"), std::wstring(L"አበባ"));
    CHECK_EQ(Type(L"selam "), std::wstring(L"ሰላም "));
    CHECK_EQ(Type(L"selam."), std::wstring(L"ሰላም."));
    CHECK_EQ(Type(L"selam,"), std::wstring(L"ሰላም,"));
    CHECK_EQ(Type(L"12"), std::wstring(L"12"));
    CHECK_EQ(Type(L"ab12"), std::wstring(L"አብ12"));

    // Unmapped ASCII (uppercase A/B/D have no Ethiopic mapping) passes through.
    CHECK_EQ(Type(L"ABD"), std::wstring(L"ABD"));

    // Repeated letters must not be lost or duplicated.
    CHECK_EQ(Type(L"ll"), std::wstring(L"ልል"));
    CHECK_EQ(Type(L"ss"), std::wstring(L"ሥ"));
}

// ---------------------------------------------------------------------------
// Composition state
// ---------------------------------------------------------------------------
static void TestCompositionState() {
    PhoneticEngine e;

    ConversionResult s = e.ProcessKey(L's');
    CHECK_TRUE(s.consumed);
    CHECK_TRUE(!s.shouldCommit);
    CHECK_EQ(s.composition, std::wstring(L"ስ"));
    CHECK_EQ(e.GetComposition(), std::wstring(L"ስ"));
    CHECK_TRUE(e.HasComposition());

    // 's' must not be committed before we know whether a longer form follows.
    ConversionResult h = e.ProcessKey(L'h');
    CHECK_TRUE(!h.shouldCommit);
    CHECK_EQ(h.composition, std::wstring(L"ሽ"));

    ConversionResult space = e.ProcessKey(L' ');
    CHECK_TRUE(space.shouldCommit);
    CHECK_EQ(space.output, std::wstring(L"ሽ "));
    CHECK_TRUE(!e.HasComposition());
}

// ---------------------------------------------------------------------------
// Backspace inside a composition
// ---------------------------------------------------------------------------
static void TestBackspace() {
    PhoneticEngine e;
    e.ProcessKey(L's');
    CHECK_EQ(e.GetComposition(), std::wstring(L"ስ"));

    e.ProcessKey(L'e');
    CHECK_EQ(e.GetComposition(), std::wstring(L"ሰ"));

    ConversionResult b1 = e.ProcessBackspace();
    CHECK_TRUE(b1.consumed);
    CHECK_EQ(e.GetComposition(), std::wstring(L"ስ"));

    ConversionResult b2 = e.ProcessBackspace();
    CHECK_TRUE(b2.consumed);
    CHECK_EQ(e.GetComposition(), std::wstring(L""));
    CHECK_TRUE(!e.HasComposition());

    ConversionResult b3 = e.ProcessBackspace();
    CHECK_TRUE(!b3.consumed);
}

// ---------------------------------------------------------------------------
// Explicit expected values for every family and order (independent of the
// mapping table, so a wrong table entry is actually caught).
// ---------------------------------------------------------------------------
struct Case {
    const wchar_t* keys;
    const wchar_t* want;
};

static const Case kCases[] = {
    // Glottal
    {L"a", L"አ"}, {L"u", L"ኡ"}, {L"i", L"ኢ"}, {L"aa", L"ኣ"},
    {L"ie", L"ኤ"}, {L"ee", L"ኤ"}, {L"e", L"እ"}, {L"o", L"ኦ"},

    // h
    {L"he", L"ሀ"}, {L"hu", L"ሁ"}, {L"hi", L"ሂ"}, {L"ha", L"ሃ"},
    {L"hie", L"ሄ"}, {L"h", L"ህ"}, {L"ho", L"ሆ"},

    // l
    {L"le", L"ለ"}, {L"lu", L"ሉ"}, {L"li", L"ሊ"}, {L"la", L"ላ"},
    {L"lie", L"ሌ"}, {L"l", L"ል"}, {L"lo", L"ሎ"}, {L"lue", L"ሏ"}, {L"lua", L"ሏ"},

    // H
    {L"He", L"ሐ"}, {L"Hu", L"ሑ"}, {L"Hi", L"ሒ"}, {L"Ha", L"ሓ"},
    {L"Hie", L"ሔ"}, {L"H", L"ሕ"}, {L"Ho", L"ሖ"}, {L"Hue", L"ሗ"}, {L"Hua", L"ሗ"},

    // m
    {L"me", L"መ"}, {L"mu", L"ሙ"}, {L"mi", L"ሚ"}, {L"ma", L"ማ"},
    {L"mie", L"ሜ"}, {L"m", L"ም"}, {L"mo", L"ሞ"}, {L"mue", L"ሟ"}, {L"mua", L"ሟ"},

    // ss
    {L"sse", L"ሠ"}, {L"ssu", L"ሡ"}, {L"ssi", L"ሢ"}, {L"ssa", L"ሣ"},
    {L"ssie", L"ሤ"}, {L"ss", L"ሥ"}, {L"sso", L"ሦ"}, {L"ssue", L"ሧ"}, {L"ssua", L"ሧ"},

    // r
    {L"re", L"ረ"}, {L"ru", L"ሩ"}, {L"ri", L"ሪ"}, {L"ra", L"ራ"},
    {L"rie", L"ሬ"}, {L"r", L"ር"}, {L"ro", L"ሮ"}, {L"rue", L"ሯ"}, {L"rua", L"ሯ"},

    // s
    {L"se", L"ሰ"}, {L"su", L"ሱ"}, {L"si", L"ሲ"}, {L"sa", L"ሳ"},
    {L"sie", L"ሴ"}, {L"s", L"ስ"}, {L"so", L"ሶ"}, {L"sue", L"ሷ"}, {L"sua", L"ሷ"},

    // x
    {L"xe", L"ሸ"}, {L"xu", L"ሹ"}, {L"xi", L"ሺ"}, {L"xa", L"ሻ"},
    {L"xie", L"ሼ"}, {L"x", L"ሽ"}, {L"xo", L"ሾ"},

    // sh alias
    {L"she", L"ሸ"}, {L"shu", L"ሹ"}, {L"shi", L"ሺ"}, {L"sha", L"ሻ"},
    {L"shie", L"ሼ"}, {L"sh", L"ሽ"}, {L"sho", L"ሾ"},

    // q
    {L"qe", L"ቀ"}, {L"qu", L"ቁ"}, {L"qi", L"ቂ"}, {L"qa", L"ቃ"},
    {L"qie", L"ቄ"}, {L"q", L"ቅ"}, {L"qo", L"ቆ"},
    {L"que", L"ቈ"}, {L"quu", L"ቍ"}, {L"qui", L"ቊ"}, {L"qua", L"ቋ"}, {L"quie", L"ቌ"},

    // b
    {L"be", L"በ"}, {L"bu", L"ቡ"}, {L"bi", L"ቢ"}, {L"ba", L"ባ"},
    {L"bie", L"ቤ"}, {L"b", L"ብ"}, {L"bo", L"ቦ"}, {L"bue", L"ቧ"}, {L"bua", L"ቧ"},

    // v
    {L"ve", L"ቨ"}, {L"vu", L"ቩ"}, {L"vi", L"ቪ"}, {L"va", L"ቫ"},
    {L"vie", L"ቬ"}, {L"v", L"ቭ"}, {L"vo", L"ቮ"}, {L"vue", L"ቯ"}, {L"vua", L"ቯ"},

    // t
    {L"te", L"ተ"}, {L"tu", L"ቱ"}, {L"ti", L"ቲ"}, {L"ta", L"ታ"},
    {L"tie", L"ቴ"}, {L"t", L"ት"}, {L"to", L"ቶ"}, {L"tue", L"ቷ"}, {L"tua", L"ቷ"},

    // c
    {L"ce", L"ቸ"}, {L"cu", L"ቹ"}, {L"ci", L"ቺ"}, {L"ca", L"ቻ"},
    {L"cie", L"ቼ"}, {L"c", L"ች"}, {L"co", L"ቾ"}, {L"cue", L"ቿ"}, {L"cua", L"ቿ"},

    // ch alias
    {L"che", L"ቸ"}, {L"chu", L"ቹ"}, {L"chi", L"ቺ"}, {L"cha", L"ቻ"},
    {L"chie", L"ቼ"}, {L"ch", L"ች"}, {L"cho", L"ቾ"}, {L"chue", L"ቿ"}, {L"chua", L"ቿ"},

    // hh
    {L"hhe", L"ኀ"}, {L"hhu", L"ኁ"}, {L"hhi", L"ኂ"}, {L"hha", L"ኃ"},
    {L"hhie", L"ኄ"}, {L"hh", L"ኅ"}, {L"hho", L"ኆ"},
    {L"hhue", L"ኈ"}, {L"hhuu", L"ኍ"}, {L"hhui", L"ኊ"}, {L"hhua", L"ኋ"}, {L"hhuie", L"ኌ"},

    // n
    {L"ne", L"ነ"}, {L"nu", L"ኑ"}, {L"ni", L"ኒ"}, {L"na", L"ና"},
    {L"nie", L"ኔ"}, {L"n", L"ን"}, {L"no", L"ኖ"}, {L"nue", L"ኗ"}, {L"nua", L"ኗ"},

    // N
    {L"Ne", L"ኘ"}, {L"Nu", L"ኙ"}, {L"Ni", L"ኚ"}, {L"Na", L"ኛ"},
    {L"Nie", L"ኜ"}, {L"N", L"ኝ"}, {L"No", L"ኞ"}, {L"Nue", L"ኟ"}, {L"Nua", L"ኟ"},

    // k
    {L"ke", L"ከ"}, {L"ku", L"ኩ"}, {L"ki", L"ኪ"}, {L"ka", L"ካ"},
    {L"kie", L"ኬ"}, {L"k", L"ክ"}, {L"ko", L"ኮ"},
    {L"kue", L"ኰ"}, {L"kuu", L"ኵ"}, {L"kui", L"ኲ"}, {L"kua", L"ኳ"}, {L"kuie", L"ኴ"},

    // K
    {L"Ke", L"ኸ"}, {L"Ku", L"ኹ"}, {L"Ki", L"ኺ"}, {L"Ka", L"ኻ"},
    {L"Kie", L"ኼ"}, {L"K", L"ኽ"}, {L"Ko", L"ኾ"},
    {L"Kue", L"ዀ"}, {L"Kuu", L"ዅ"}, {L"Kui", L"ዂ"}, {L"Kua", L"ዃ"}, {L"Kuie", L"ዄ"},

    // w
    {L"we", L"ወ"}, {L"wu", L"ዉ"}, {L"wi", L"ዊ"}, {L"wa", L"ዋ"},
    {L"wie", L"ዌ"}, {L"w", L"ው"}, {L"wo", L"ዎ"},

    // z
    {L"ze", L"ዘ"}, {L"zu", L"ዙ"}, {L"zi", L"ዚ"}, {L"za", L"ዛ"},
    {L"zie", L"ዜ"}, {L"z", L"ዝ"}, {L"zo", L"ዞ"}, {L"zue", L"ዟ"}, {L"zua", L"ዟ"},

    // Z
    {L"Ze", L"ዠ"}, {L"Zu", L"ዡ"}, {L"Zi", L"ዢ"}, {L"Za", L"ዣ"},
    {L"Zie", L"ዤ"}, {L"Z", L"ዥ"}, {L"Zo", L"ዦ"}, {L"Zue", L"ዧ"}, {L"Zua", L"ዧ"},

    // y
    {L"ye", L"የ"}, {L"yu", L"ዩ"}, {L"yi", L"ዪ"}, {L"ya", L"ያ"},
    {L"yie", L"ዬ"}, {L"y", L"ይ"}, {L"yo", L"ዮ"},

    // d
    {L"de", L"ደ"}, {L"du", L"ዱ"}, {L"di", L"ዲ"}, {L"da", L"ዳ"},
    {L"die", L"ዴ"}, {L"d", L"ድ"}, {L"do", L"ዶ"}, {L"due", L"ዷ"}, {L"dua", L"ዷ"},

    // j
    {L"je", L"ጀ"}, {L"ju", L"ጁ"}, {L"ji", L"ጂ"}, {L"ja", L"ጃ"},
    {L"jie", L"ጄ"}, {L"j", L"ጅ"}, {L"jo", L"ጆ"}, {L"jue", L"ጇ"}, {L"jua", L"ጇ"},

    // g
    {L"ge", L"ገ"}, {L"gu", L"ጉ"}, {L"gi", L"ጊ"}, {L"ga", L"ጋ"},
    {L"gie", L"ጌ"}, {L"g", L"ግ"}, {L"go", L"ጎ"},
    {L"gue", L"ጐ"}, {L"guu", L"ጕ"}, {L"gui", L"ጒ"}, {L"gua", L"ጓ"}, {L"guie", L"ጔ"},

    // T
    {L"Te", L"ጠ"}, {L"Tu", L"ጡ"}, {L"Ti", L"ጢ"}, {L"Ta", L"ጣ"},
    {L"Tie", L"ጤ"}, {L"T", L"ጥ"}, {L"To", L"ጦ"}, {L"Tue", L"ጧ"}, {L"Tua", L"ጧ"},

    // C
    {L"Ce", L"ጨ"}, {L"Cu", L"ጩ"}, {L"Ci", L"ጪ"}, {L"Ca", L"ጫ"},
    {L"Cie", L"ጬ"}, {L"C", L"ጭ"}, {L"Co", L"ጮ"}, {L"Cue", L"ጯ"}, {L"Cua", L"ጯ"},

    // P
    {L"Pe", L"ጰ"}, {L"Pu", L"ጱ"}, {L"Pi", L"ጲ"}, {L"Pa", L"ጳ"},
    {L"Pie", L"ጴ"}, {L"P", L"ጵ"}, {L"Po", L"ጶ"}, {L"Pue", L"ጷ"}, {L"Pua", L"ጷ"},

    // S
    {L"Se", L"ጸ"}, {L"Su", L"ጹ"}, {L"Si", L"ጺ"}, {L"Sa", L"ጻ"},
    {L"Sie", L"ጼ"}, {L"S", L"ጽ"}, {L"So", L"ጾ"}, {L"Sue", L"ጿ"}, {L"Sua", L"ጿ"},

    // SS
    {L"SSe", L"ፀ"}, {L"SSu", L"ፁ"}, {L"SSi", L"ፂ"}, {L"SSa", L"ፃ"},
    {L"SSie", L"ፄ"}, {L"SS", L"ፅ"}, {L"SSo", L"ፆ"},

    // f
    {L"fe", L"ፈ"}, {L"fu", L"ፉ"}, {L"fi", L"ፊ"}, {L"fa", L"ፋ"},
    {L"fie", L"ፌ"}, {L"f", L"ፍ"}, {L"fo", L"ፎ"}, {L"fue", L"ፏ"}, {L"fua", L"ፏ"},

    // p
    {L"pe", L"ፐ"}, {L"pu", L"ፑ"}, {L"pi", L"ፒ"}, {L"pa", L"ፓ"},
    {L"pie", L"ፔ"}, {L"p", L"ፕ"}, {L"po", L"ፖ"}, {L"pue", L"ፗ"}, {L"pua", L"ፗ"},
};

static void TestMappingCases() {
    for (const Case& c : kCases) {
        std::wstring got = Type(c.keys);
        if (got != std::wstring(c.want)) {
            ++g_failures;
            std::cerr << "FAIL mapping: typed [" << U8(c.keys) << "] got ["
                      << U8(got) << "] expected [" << U8(c.want) << "]\n";
        }
        ++g_checks;
    }
}

// ---------------------------------------------------------------------------
// Exhaustive consistency: every table entry must round-trip through the engine.
// This also proves no duplicate keys silently overwrite each other.
// ---------------------------------------------------------------------------
static void TestEveryTableEntry() {
    MappingTable table;
    table.LoadGFFPhonetic();

    CHECK_TRUE(table.GetAllMappings().size() > 300);

    std::set<std::wstring> seen;
    for (const MappingEntry& m : table.GetAllMappings()) {
        if (!seen.insert(m.latin).second) {
            ++g_failures;
            std::cerr << "FAIL duplicate mapping key [" << U8(m.latin) << "]\n";
        }
        std::wstring got = Type(m.latin);
        if (got != m.ethiopic) {
            ++g_failures;
            std::cerr << "FAIL table entry [" << U8(m.latin) << "] got ["
                      << U8(got) << "] expected [" << U8(m.ethiopic) << "]\n";
        }
        ++g_checks;
    }
}

int main() {
    std::cout << "FidelKey engine tests\n";

    TestWords();
    TestCompositionState();
    TestBackspace();
    TestMappingCases();
    TestEveryTableEntry();

    std::cout << "Checks: " << g_checks << ", failures: " << g_failures << "\n";
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << "TESTS FAILED\n";
    return 1;
}
