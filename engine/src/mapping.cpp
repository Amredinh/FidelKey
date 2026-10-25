#include "fidelkey/mapping.h"

namespace fidelkey {
namespace engine {

MappingTable::MappingTable() {
}

void MappingTable::LoadGFFPhonetic() {
    mappings.clear();
    
    // Vowels
    AddMapping(L"a", L"አ");
    AddMapping(L"u", L"ኡ");
    AddMapping(L"i", L"ኢ");
    AddMapping(L"e", L"ኤ");
    AddMapping(L"E", L"እ"); // glottal stop
    AddMapping(L"o", L"ኦ");
    
    // h family
    AddMapping(L"he", L"ሀ");
    AddMapping(L"hu", L"ሁ");
    AddMapping(L"hi", L"ሂ");
    AddMapping(L"ha", L"ሃ");
    AddMapping(L"hie", L"ሄ");
    AddMapping(L"h", L"ህ"); // 6th order
    AddMapping(L"ho", L"ሆ");
    
    // l family
    AddMapping(L"le", L"ለ");
    AddMapping(L"lu", L"ሉ");
    AddMapping(L"li", L"ሊ");
    AddMapping(L"la", L"ላ");
    AddMapping(L"lie", L"ሌ");
    AddMapping(L"lee", L"ሌ");
    AddMapping(L"l", L"ል");
    AddMapping(L"lo", L"ሎ");
    AddMapping(L"lua", L"ሏ");
    AddMapping(L"lwa", L"ሏ");
    
    // H/hh family (ሐ)
    AddMapping(L"He", L"ሐ");
    AddMapping(L"Hu", L"ሑ");
    AddMapping(L"Hi", L"ሒ");
    AddMapping(L"Ha", L"ሓ");
    AddMapping(L"Hie", L"ሔ");
    AddMapping(L"Hh", L"ሕ");
    AddMapping(L"H", L"ሕ");
    AddMapping(L"Ho", L"ሖ");
    AddMapping(L"Hua", L"ሗ");
    AddMapping(L"Hwa", L"ሗ");
    
    // m family
    AddMapping(L"me", L"መ");
    AddMapping(L"mu", L"ሙ");
    AddMapping(L"mi", L"ሚ");
    AddMapping(L"ma", L"ማ");
    AddMapping(L"mie", L"ሜ");
    AddMapping(L"mee", L"ሜ");
    AddMapping(L"m", L"ም");
    AddMapping(L"mo", L"ሞ");
    AddMapping(L"mua", L"ሟ");
    AddMapping(L"mwa", L"ሟ");
    
    // s family
    AddMapping(L"se", L"ሰ");
    AddMapping(L"su", L"ሱ");
    AddMapping(L"si", L"ሲ");
    AddMapping(L"sa", L"ሳ");
    AddMapping(L"sie", L"ሴ");
    AddMapping(L"see", L"ሴ");
    AddMapping(L"s", L"ስ");
    AddMapping(L"so", L"ሶ");
    AddMapping(L"sua", L"ሷ");
    AddMapping(L"swa", L"ሷ");
    
    // S/ss family (ሠ)
    AddMapping(L"Se", L"ሠ");
    AddMapping(L"Su", L"ሡ");
    AddMapping(L"Si", L"ሢ");
    AddMapping(L"Sa", L"ሣ");
    AddMapping(L"Sie", L"ሤ");
    AddMapping(L"Ss", L"ሥ");
    AddMapping(L"S", L"ሥ");
    AddMapping(L"So", L"ሦ");
    AddMapping(L"Sua", L"ሧ");
    AddMapping(L"Swa", L"ሧ");
    
    // r family
    AddMapping(L"re", L"ረ");
    AddMapping(L"ru", L"ሩ");
    AddMapping(L"ri", L"ሪ");
    AddMapping(L"ra", L"ራ");
    AddMapping(L"rie", L"ሬ");
    AddMapping(L"ree", L"ሬ");
    AddMapping(L"r", L"ር");
    AddMapping(L"ro", L"ሮ");
    AddMapping(L"rua", L"ሯ");
    AddMapping(L"rwa", L"ሯ");
    
    // sh family
    AddMapping(L"she", L"ሸ");
    AddMapping(L"shu", L"ሹ");
    AddMapping(L"shi", L"ሺ");
    AddMapping(L"sha", L"ሻ");
    AddMapping(L"shie", L"ሼ");
    AddMapping(L"shee", L"ሼ");
    AddMapping(L"sh", L"ሽ");
    AddMapping(L"sho", L"ሾ");
    AddMapping(L"shua", L"ሿ");
    AddMapping(L"shwa", L"ሿ");
    
    // q family
    AddMapping(L"qe", L"ቀ");
    AddMapping(L"qu", L"ቁ");
    AddMapping(L"qi", L"ቂ");
    AddMapping(L"qa", L"ቃ");
    AddMapping(L"qie", L"ቄ");
    AddMapping(L"qee", L"ቄ");
    AddMapping(L"q", L"ቅ");
    AddMapping(L"qo", L"ቆ");
    AddMapping(L"qua", L"ቋ");
    AddMapping(L"qwa", L"ቋ");
    
    // b family
    AddMapping(L"be", L"በ");
    AddMapping(L"bu", L"ቡ");
    AddMapping(L"bi", L"ቢ");
    AddMapping(L"ba", L"ባ");
    AddMapping(L"bie", L"ቤ");
    AddMapping(L"bee", L"ቤ");
    AddMapping(L"b", L"ብ");
    AddMapping(L"bo", L"ቦ");
    AddMapping(L"bua", L"ቧ");
    AddMapping(L"bwa", L"ቧ");
    
    // t family
    AddMapping(L"te", L"ተ");
    AddMapping(L"tu", L"ቱ");
    AddMapping(L"ti", L"ቲ");
    AddMapping(L"ta", L"ታ");
    AddMapping(L"tie", L"ቴ");
    AddMapping(L"tee", L"ቴ");
    AddMapping(L"t", L"ት");
    AddMapping(L"to", L"ቶ");
    AddMapping(L"tua", L"ቷ");
    AddMapping(L"twa", L"ቷ");
    
    // ch/c family
    AddMapping(L"che", L"ቸ");
    AddMapping(L"chu", L"ቹ");
    AddMapping(L"chi", L"ቺ");
    AddMapping(L"cha", L"ቻ");
    AddMapping(L"chie", L"ቼ");
    AddMapping(L"chee", L"ቼ");
    AddMapping(L"ch", L"ች");
    AddMapping(L"cho", L"ቾ");
    AddMapping(L"chua", L"ቿ");
    AddMapping(L"chwa", L"ቿ");
    AddMapping(L"ce", L"ቸ");
    AddMapping(L"cu", L"ቹ");
    AddMapping(L"ci", L"ቺ");
    AddMapping(L"ca", L"ቻ");
    AddMapping(L"cie", L"ቼ");
    AddMapping(L"c", L"ች");
    AddMapping(L"co", L"ቾ");
    AddMapping(L"cua", L"ቿ");
    AddMapping(L"cwa", L"ቿ");
    
    // T/tt family (ጠ)
    AddMapping(L"Te", L"ጠ");
    AddMapping(L"Tu", L"ጡ");
    AddMapping(L"Ti", L"ጢ");
    AddMapping(L"Ta", L"ጣ");
    AddMapping(L"Tie", L"ጤ");
    AddMapping(L"Tee", L"ጤ");
    AddMapping(L"Tt", L"ጥ");
    AddMapping(L"T", L"ጥ");
    AddMapping(L"To", L"ጦ");
    AddMapping(L"Tua", L"ጧ");
    AddMapping(L"Twa", L"ጧ");
    
    // C/cc family (ጨ)
    AddMapping(L"Che", L"ጨ");
    AddMapping(L"Chu", L"ጩ");
    AddMapping(L"Chi", L"ጪ");
    AddMapping(L"Cha", L"ጫ");
    AddMapping(L"Chie", L"ጬ");
    AddMapping(L"Chee", L"ጬ");
    AddMapping(L"Ch", L"ጭ");
    AddMapping(L"Cho", L"ጮ");
    AddMapping(L"Chua", L"ጯ");
    AddMapping(L"Chwa", L"ጯ");
    
    // n family
    AddMapping(L"ne", L"ነ");
    AddMapping(L"nu", L"ኑ");
    AddMapping(L"ni", L"ኒ");
    AddMapping(L"na", L"ና");
    AddMapping(L"nie", L"ኔ");
    AddMapping(L"nee", L"ኔ");
    AddMapping(L"n", L"ን");
    AddMapping(L"no", L"ኖ");
    AddMapping(L"nua", L"ኗ");
    AddMapping(L"nwa", L"ኗ");
    
    // N/ny family
    AddMapping(L"Ne", L"ኘ");
    AddMapping(L"Nu", L"ኙ");
    AddMapping(L"Ni", L"ኚ");
    AddMapping(L"Na", L"ኛ");
    AddMapping(L"Nie", L"ኜ");
    AddMapping(L"N", L"ኝ");
    AddMapping(L"No", L"ኞ");
    AddMapping(L"Nua", L"ኟ");
    AddMapping(L"Nwa", L"ኟ");
    
    // k family
    AddMapping(L"ke", L"ከ");
    AddMapping(L"ku", L"ኩ");
    AddMapping(L"ki", L"ኪ");
    AddMapping(L"ka", L"ካ");
    AddMapping(L"kie", L"ኬ");
    AddMapping(L"kee", L"ኬ");
    AddMapping(L"k", L"ክ");
    AddMapping(L"ko", L"ኮ");
    AddMapping(L"kua", L"ኳ");
    AddMapping(L"kwa", L"ኳ");
    
    // K/kk family (ኸ)
    AddMapping(L"Ke", L"ኸ");
    AddMapping(L"Ku", L"ኹ");
    AddMapping(L"Ki", L"ኺ");
    AddMapping(L"Ka", L"ኻ");
    AddMapping(L"Kie", L"ኼ");
    AddMapping(L"K", L"ኽ");
    AddMapping(L"Ko", L"ኾ");
    AddMapping(L"Kua", L"ኋ");
    AddMapping(L"Kwa", L"ኋ");
    
    // w family
    AddMapping(L"we", L"ወ");
    AddMapping(L"wu", L"ዉ");
    AddMapping(L"wi", L"ዊ");
    AddMapping(L"wa", L"ዋ");
    AddMapping(L"wie", L"ዌ");
    AddMapping(L"wee", L"ዌ");
    AddMapping(L"w", L"ው");
    AddMapping(L"wo", L"ዎ");
    
    // z family
    AddMapping(L"ze", L"ዘ");
    AddMapping(L"zu", L"ዙ");
    AddMapping(L"zi", L"ዚ");
    AddMapping(L"za", L"ዛ");
    AddMapping(L"zie", L"ዜ");
    AddMapping(L"zee", L"ዜ");
    AddMapping(L"z", L"ዝ");
    AddMapping(L"zo", L"ዞ");
    AddMapping(L"zua", L"ዟ");
    AddMapping(L"zwa", L"ዟ");
    
    // Z/zh family
    AddMapping(L"Ze", L"ዠ");
    AddMapping(L"Zu", L"ዡ");
    AddMapping(L"Zi", L"ዢ");
    AddMapping(L"Za", L"ዣ");
    AddMapping(L"Zie", L"ዤ");
    AddMapping(L"Z", L"ዥ");
    AddMapping(L"Zo", L"ዦ");
    AddMapping(L"Zua", L"ዧ");
    AddMapping(L"Zwa", L"ዧ");
    
    // y family
    AddMapping(L"ye", L"የ");
    AddMapping(L"yu", L"ዩ");
    AddMapping(L"yi", L"ዪ");
    AddMapping(L"ya", L"ያ");
    AddMapping(L"yie", L"ዬ");
    AddMapping(L"yee", L"ዬ");
    AddMapping(L"y", L"ይ");
    AddMapping(L"yo", L"ዮ");
    
    // d family
    AddMapping(L"de", L"ደ");
    AddMapping(L"du", L"ዱ");
    AddMapping(L"di", L"ዲ");
    AddMapping(L"da", L"ዳ");
    AddMapping(L"die", L"ዴ");
    AddMapping(L"dee", L"ዴ");
    AddMapping(L"d", L"ድ");
    AddMapping(L"do", L"ዶ");
    AddMapping(L"dua", L"ዷ");
    AddMapping(L"dwa", L"ዷ");
    
    // j family
    AddMapping(L"je", L"ጀ");
    AddMapping(L"ju", L"ጁ");
    AddMapping(L"ji", L"ጂ");
    AddMapping(L"ja", L"ጃ");
    AddMapping(L"jie", L"ጄ");
    AddMapping(L"jee", L"ጄ");
    AddMapping(L"j", L"ጅ");
    AddMapping(L"jo", L"ጆ");
    AddMapping(L"jua", L"ጇ");
    AddMapping(L"jwa", L"ጇ");
    
    // g family
    AddMapping(L"ge", L"ገ");
    AddMapping(L"gu", L"ጉ");
    AddMapping(L"gi", L"ጊ");
    AddMapping(L"ga", L"ጋ");
    AddMapping(L"gie", L"ጌ");
    AddMapping(L"gee", L"ጌ");
    AddMapping(L"g", L"ግ");
    AddMapping(L"go", L"ጎ");
    AddMapping(L"gua", L"ጓ");
    AddMapping(L"gwa", L"ጓ");
    AddMapping(L"guie", L"ጔ");
    
    // G/gg family
    AddMapping(L"Ge", L"ጘ");
    AddMapping(L"Gu", L"ጙ");
    AddMapping(L"Gi", L"ጚ");
    AddMapping(L"Ga", L"ጛ");
    AddMapping(L"Gie", L"ጜ");
    AddMapping(L"G", L"ጝ");
    AddMapping(L"Go", L"ጞ");
    AddMapping(L"Gua", L"ⶓ");
    
    // P/pp family (ጰ)
    AddMapping(L"Pe", L"ጰ");
    AddMapping(L"Pu", L"ጱ");
    AddMapping(L"Pi", L"ጲ");
    AddMapping(L"Pa", L"ጳ");
    AddMapping(L"Pie", L"ጴ");
    AddMapping(L"P", L"ጵ");
    AddMapping(L"Po", L"ጶ");
    AddMapping(L"Pua", L"ጷ");
    
    // ts family (ጸ)
    AddMapping(L"tse", L"ጸ");
    AddMapping(L"tsu", L"ጹ");
    AddMapping(L"tsi", L"ጺ");
    AddMapping(L"tsa", L"ጻ");
    AddMapping(L"tsie", L"ጼ");
    AddMapping(L"tse", L"ጼ");
    AddMapping(L"ts", L"ጽ");
    AddMapping(L"tso", L"ጾ");
    AddMapping(L"tsua", L"ጿ");
    
    // f family
    AddMapping(L"fe", L"ፈ");
    AddMapping(L"fu", L"ፉ");
    AddMapping(L"fi", L"ፊ");
    AddMapping(L"fa", L"ፋ");
    AddMapping(L"fie", L"ፌ");
    AddMapping(L"fee", L"ፌ");
    AddMapping(L"f", L"ፍ");
    AddMapping(L"fo", L"ፎ");
    AddMapping(L"fua", L"ፏ");
    AddMapping(L"fwa", L"ፏ");
    
    // p family
    AddMapping(L"pe", L"ፐ");
    AddMapping(L"pu", L"ፑ");
    AddMapping(L"pi", L"ፒ");
    AddMapping(L"pa", L"ፓ");
    AddMapping(L"pie", L"ፔ");
    AddMapping(L"pee", L"ፔ");
    AddMapping(L"p", L"ፕ");
    AddMapping(L"po", L"ፖ");
    AddMapping(L"pua", L"ፗ");
    AddMapping(L"pwa", L"ፗ");
}

const std::vector<MappingEntry>& MappingTable::GetAllMappings() const {
    return mappings;
}

void MappingTable::AddMapping(const std::wstring& latin, const std::wstring& ethiopic, int priority) {
    mappings.push_back({latin, ethiopic, priority});
}

} // namespace engine
} // namespace fidelkey
