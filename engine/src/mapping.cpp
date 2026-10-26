#include "fidelkey/mapping.h"

namespace fidelkey {
namespace engine {

MappingTable::MappingTable() {
}

// Mapping derived from the GFF Amharic 3.3 keyboard typing table:
// https://help.keyman.com/keyboard/gff_amharic/3.3/gff_amharic
//
// Each consonant family is typed as <base> followed by a vowel:
//   +e   -> 1st order (Ge'ez)
//   +u   -> 2nd order (Ka'eb)
//   +i   -> 3rd order (Salis)
//   +a   -> 4th order (Rabe)
//   +ie  -> 5th order (Hames)   (also accepted: +ee)
//   (bare)-> 6th order (Sades)
//   +o   -> 7th order (Sabe)
// Labiovelar forms append u+vowel: +ue +uu +ui +ua +uie.
namespace {

void AddFamily(MappingTable& table, const wchar_t* b,
    const wchar_t* o1, const wchar_t* o2, const wchar_t* o3, const wchar_t* o4,
    const wchar_t* o5, const wchar_t* o6, const wchar_t* o7,
    const wchar_t* l1 = nullptr, const wchar_t* l2 = nullptr,
    const wchar_t* l3 = nullptr, const wchar_t* l4 = nullptr,
    const wchar_t* l5 = nullptr) {
    std::wstring base(b);
    auto add = [&table](const std::wstring& key, const wchar_t* value) {
        if (value != nullptr) table.AddMapping(key, value);
    };
    add(base + L"e", o1);
    add(base + L"u", o2);
    add(base + L"i", o3);
    add(base + L"a", o4);
    if (o5 != nullptr) {
        add(base + L"ie", o5);
        add(base + L"ee", o5);
    }
    add(base, o6);
    add(base + L"o", o7);
    add(base + L"ue", l1);
    add(base + L"uu", l2);
    add(base + L"ui", l3);
    add(base + L"ua", l4);
    add(base + L"uie", l5);
}

} // namespace

void MappingTable::LoadGFFPhonetic() {
    mappings.clear();

    // Glottal (አ family): standalone vowels. GFF types 1st order as "a",
    // 4th order as "aa", 6th order as "e", 5th order as "ie"/"ee".
    AddMapping(L"a", L"አ");
    AddMapping(L"u", L"ኡ");
    AddMapping(L"i", L"ኢ");
    AddMapping(L"aa", L"ኣ");
    AddMapping(L"ie", L"ኤ");
    AddMapping(L"ee", L"ኤ");
    AddMapping(L"e", L"እ");
    AddMapping(L"o", L"ኦ");

    // h family
    AddFamily(*this, L"h", L"ሀ", L"ሁ", L"ሂ", L"ሃ", L"ሄ", L"ህ", L"ሆ");

    // l family
    AddFamily(*this, L"l", L"ለ", L"ሉ", L"ሊ", L"ላ", L"ሌ", L"ል", L"ሎ",
        L"ሏ", nullptr, nullptr, L"ሏ");

    // H family (ሐ)
    AddFamily(*this, L"H", L"ሐ", L"ሑ", L"ሒ", L"ሓ", L"ሔ", L"ሕ", L"ሖ",
        L"ሗ", nullptr, nullptr, L"ሗ");

    // m family
    AddFamily(*this, L"m", L"መ", L"ሙ", L"ሚ", L"ማ", L"ሜ", L"ም", L"ሞ",
        L"ሟ", nullptr, nullptr, L"ሟ");

    // ss family (ሠ)
    AddFamily(*this, L"ss", L"ሠ", L"ሡ", L"ሢ", L"ሣ", L"ሤ", L"ሥ", L"ሦ",
        L"ሧ", nullptr, nullptr, L"ሧ");

    // r family
    AddFamily(*this, L"r", L"ረ", L"ሩ", L"ሪ", L"ራ", L"ሬ", L"ር", L"ሮ",
        L"ሯ", nullptr, nullptr, L"ሯ");

    // s family (ሰ)
    AddFamily(*this, L"s", L"ሰ", L"ሱ", L"ሲ", L"ሳ", L"ሴ", L"ስ", L"ሶ",
        L"ሷ", nullptr, nullptr, L"ሷ");

    // x family (ሸ). GFF uses 'x'; 'sh' is kept as a documented convenience alias.
    AddFamily(*this, L"x", L"ሸ", L"ሹ", L"ሺ", L"ሻ", L"ሼ", L"ሽ", L"ሾ",
        L"ሿ", nullptr, nullptr, L"ሿ");
    AddFamily(*this, L"sh", L"ሸ", L"ሹ", L"ሺ", L"ሻ", L"ሼ", L"ሽ", L"ሾ",
        L"ሿ", nullptr, nullptr, L"ሿ");

    // q family with labiovelars
    AddFamily(*this, L"q", L"ቀ", L"ቁ", L"ቂ", L"ቃ", L"ቄ", L"ቅ", L"ቆ",
        L"ቈ", L"ቍ", L"ቊ", L"ቋ", L"ቌ");

    // b family
    AddFamily(*this, L"b", L"በ", L"ቡ", L"ቢ", L"ባ", L"ቤ", L"ብ", L"ቦ",
        L"ቧ", nullptr, nullptr, L"ቧ");

    // v family
    AddFamily(*this, L"v", L"ቨ", L"ቩ", L"ቪ", L"ቫ", L"ቬ", L"ቭ", L"ቮ",
        L"ቯ", nullptr, nullptr, L"ቯ");

    // t family
    AddFamily(*this, L"t", L"ተ", L"ቱ", L"ቲ", L"ታ", L"ቴ", L"ት", L"ቶ",
        L"ቷ", nullptr, nullptr, L"ቷ");

    // c family (ቸ). GFF uses 'c'; 'ch' is a documented convenience alias.
    AddFamily(*this, L"c", L"ቸ", L"ቹ", L"ቺ", L"ቻ", L"ቼ", L"ች", L"ቾ",
        L"ቿ", nullptr, nullptr, L"ቿ");
    AddFamily(*this, L"ch", L"ቸ", L"ቹ", L"ቺ", L"ቻ", L"ቼ", L"ች", L"ቾ",
        L"ቿ", nullptr, nullptr, L"ቿ");

    // hh family (ኀ) with labiovelars
    AddFamily(*this, L"hh", L"ኀ", L"ኁ", L"ኂ", L"ኃ", L"ኄ", L"ኅ", L"ኆ",
        L"ኈ", L"ኍ", L"ኊ", L"ኋ", L"ኌ");

    // n family
    AddFamily(*this, L"n", L"ነ", L"ኑ", L"ኒ", L"ና", L"ኔ", L"ን", L"ኖ",
        L"ኗ", nullptr, nullptr, L"ኗ");

    // N family (ኘ)
    AddFamily(*this, L"N", L"ኘ", L"ኙ", L"ኚ", L"ኛ", L"ኜ", L"ኝ", L"ኞ",
        L"ኟ", nullptr, nullptr, L"ኟ");

    // k family with labiovelars
    AddFamily(*this, L"k", L"ከ", L"ኩ", L"ኪ", L"ካ", L"ኬ", L"ክ", L"ኮ",
        L"ኰ", L"ኵ", L"ኲ", L"ኳ", L"ኴ");

    // K family (ኸ) with labiovelars
    AddFamily(*this, L"K", L"ኸ", L"ኹ", L"ኺ", L"ኻ", L"ኼ", L"ኽ", L"ኾ",
        L"ዀ", L"ዅ", L"ዂ", L"ዃ", L"ዄ");

    // w family
    AddFamily(*this, L"w", L"ወ", L"ዉ", L"ዊ", L"ዋ", L"ዌ", L"ው", L"ዎ");

    // z family
    AddFamily(*this, L"z", L"ዘ", L"ዙ", L"ዚ", L"ዛ", L"ዜ", L"ዝ", L"ዞ",
        L"ዟ", nullptr, nullptr, L"ዟ");

    // Z family
    AddFamily(*this, L"Z", L"ዠ", L"ዡ", L"ዢ", L"ዣ", L"ዤ", L"ዥ", L"ዦ",
        L"ዧ", nullptr, nullptr, L"ዧ");

    // y family
    AddFamily(*this, L"y", L"የ", L"ዩ", L"ዪ", L"ያ", L"ዬ", L"ይ", L"ዮ");

    // d family
    AddFamily(*this, L"d", L"ደ", L"ዱ", L"ዲ", L"ዳ", L"ዴ", L"ድ", L"ዶ",
        L"ዷ", nullptr, nullptr, L"ዷ");

    // j family
    AddFamily(*this, L"j", L"ጀ", L"ጁ", L"ጂ", L"ጃ", L"ጄ", L"ጅ", L"ጆ",
        L"ጇ", nullptr, nullptr, L"ጇ");

    // g family with labiovelars
    AddFamily(*this, L"g", L"ገ", L"ጉ", L"ጊ", L"ጋ", L"ጌ", L"ግ", L"ጎ",
        L"ጐ", L"ጕ", L"ጒ", L"ጓ", L"ጔ");

    // T family (ጠ)
    AddFamily(*this, L"T", L"ጠ", L"ጡ", L"ጢ", L"ጣ", L"ጤ", L"ጥ", L"ጦ",
        L"ጧ", nullptr, nullptr, L"ጧ");

    // C family (ጨ)
    AddFamily(*this, L"C", L"ጨ", L"ጩ", L"ጪ", L"ጫ", L"ጬ", L"ጭ", L"ጮ",
        L"ጯ", nullptr, nullptr, L"ጯ");

    // P family (ጰ)
    AddFamily(*this, L"P", L"ጰ", L"ጱ", L"ጲ", L"ጳ", L"ጴ", L"ጵ", L"ጶ",
        L"ጷ", nullptr, nullptr, L"ጷ");

    // S family (ጸ)
    AddFamily(*this, L"S", L"ጸ", L"ጹ", L"ጺ", L"ጻ", L"ጼ", L"ጽ", L"ጾ",
        L"ጿ", nullptr, nullptr, L"ጿ");

    // SS family (ፀ)
    AddFamily(*this, L"SS", L"ፀ", L"ፁ", L"ፂ", L"ፃ", L"ፄ", L"ፅ", L"ፆ");

    // f family
    AddFamily(*this, L"f", L"ፈ", L"ፉ", L"ፊ", L"ፋ", L"ፌ", L"ፍ", L"ፎ",
        L"ፏ", nullptr, nullptr, L"ፏ");

    // p family
    AddFamily(*this, L"p", L"ፐ", L"ፑ", L"ፒ", L"ፓ", L"ፔ", L"ፕ", L"ፖ",
        L"ፗ", nullptr, nullptr, L"ፗ");

    // NOTE: The ዐ (pharyngeal) family and non-Amharic families (ጘ, ⶠ, ...) are
    // intentionally omitted; GFF does not expose them on the Amharic layout.
}

const std::vector<MappingEntry>& MappingTable::GetAllMappings() const {
    return mappings;
}

void MappingTable::AddMapping(const std::wstring& latin, const std::wstring& ethiopic, int priority) {
    mappings.push_back({latin, ethiopic, priority});
}

} // namespace engine
} // namespace fidelkey
