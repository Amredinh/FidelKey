import { useEffect, useState } from "react";
import type { CSSProperties } from "react";
import { invoke } from "@tauri-apps/api/core";

type Screen = "home" | "settings" | "reference" | "about";
type Layout = "phonetic" | "traditional";

interface Settings {
  layout: Layout;
  show_typing_test: boolean;
}

interface KeyboardStatus {
  installed: boolean;
  packaged: boolean;
}

interface Family {
  key: string;
  forms: string[];
}

const VOWEL_HEADER = ["e", "u", "i", "a", "ie", "—", "o"];

const FAMILIES: Family[] = [
  { key: "h", forms: ["ሀ", "ሁ", "ሂ", "ሃ", "ሄ", "ህ", "ሆ"] },
  { key: "l", forms: ["ለ", "ሉ", "ሊ", "ላ", "ሌ", "ል", "ሎ"] },
  { key: "H", forms: ["ሐ", "ሑ", "ሒ", "ሓ", "ሔ", "ሕ", "ሖ"] },
  { key: "m", forms: ["መ", "ሙ", "ሚ", "ማ", "ሜ", "ም", "ሞ"] },
  { key: "ss", forms: ["ሠ", "ሡ", "ሢ", "ሣ", "ሤ", "ሥ", "ሦ"] },
  { key: "r", forms: ["ረ", "ሩ", "ሪ", "ራ", "ሬ", "ር", "ሮ"] },
  { key: "s", forms: ["ሰ", "ሱ", "ሲ", "ሳ", "ሴ", "ስ", "ሶ"] },
  { key: "x  (alias: sh)", forms: ["ሸ", "ሹ", "ሺ", "ሻ", "ሼ", "ሽ", "ሾ"] },
  { key: "q", forms: ["ቀ", "ቁ", "ቂ", "ቃ", "ቄ", "ቅ", "ቆ"] },
  { key: "b", forms: ["በ", "ቡ", "ቢ", "ባ", "ቤ", "ብ", "ቦ"] },
  { key: "v", forms: ["ቨ", "ቩ", "ቪ", "ቫ", "ቬ", "ቭ", "ቮ"] },
  { key: "t", forms: ["ተ", "ቱ", "ቲ", "ታ", "ቴ", "ት", "ቶ"] },
  { key: "c  (alias: ch)", forms: ["ቸ", "ቹ", "ቺ", "ቻ", "ቼ", "ች", "ቾ"] },
  { key: "hh", forms: ["ኀ", "ኁ", "ኂ", "ኃ", "ኄ", "ኅ", "ኆ"] },
  { key: "n", forms: ["ነ", "ኑ", "ኒ", "ና", "ኔ", "ን", "ኖ"] },
  { key: "N", forms: ["ኘ", "ኙ", "ኚ", "ኛ", "ኜ", "ኝ", "ኞ"] },
  { key: "k", forms: ["ከ", "ኩ", "ኪ", "ካ", "ኬ", "ክ", "ኮ"] },
  { key: "K", forms: ["ኸ", "ኹ", "ኺ", "ኻ", "ኼ", "ኽ", "ኾ"] },
  { key: "w", forms: ["ወ", "ዉ", "ዊ", "ዋ", "ዌ", "ው", "ዎ"] },
  { key: "z", forms: ["ዘ", "ዙ", "ዚ", "ዛ", "ዜ", "ዝ", "ዞ"] },
  { key: "Z", forms: ["ዠ", "ዡ", "ዢ", "ዣ", "ዤ", "ዥ", "ዦ"] },
  { key: "y", forms: ["የ", "ዩ", "ዪ", "ያ", "ዬ", "ይ", "ዮ"] },
  { key: "d", forms: ["ደ", "ዱ", "ዲ", "ዳ", "ዴ", "ድ", "ዶ"] },
  { key: "j", forms: ["ጀ", "ጁ", "ጂ", "ጃ", "ጄ", "ጅ", "ጆ"] },
  { key: "g", forms: ["ገ", "ጉ", "ጊ", "ጋ", "ጌ", "ግ", "ጎ"] },
  { key: "T", forms: ["ጠ", "ጡ", "ጢ", "ጣ", "ጤ", "ጥ", "ጦ"] },
  { key: "C", forms: ["ጨ", "ጩ", "ጪ", "ጫ", "ጬ", "ጭ", "ጮ"] },
  { key: "P", forms: ["ጰ", "ጱ", "ጲ", "ጳ", "ጴ", "ጵ", "ጶ"] },
  { key: "S", forms: ["ጸ", "ጹ", "ጺ", "ጻ", "ጼ", "ጽ", "ጾ"] },
  { key: "SS", forms: ["ፀ", "ፁ", "ፂ", "ፃ", "ፄ", "ፅ", "ፆ"] },
  { key: "f", forms: ["ፈ", "ፉ", "ፊ", "ፋ", "ፌ", "ፍ", "ፎ"] },
  { key: "p", forms: ["ፐ", "ፑ", "ፒ", "ፓ", "ፔ", "ፕ", "ፖ"] },
];

const LABIOVELARS = [
  ["q", "que ቈ   quu ቍ   qui ቊ   qua ቋ   quie ቌ"],
  ["hh", "hhue ኈ   hhuu ኍ   hhui ኊ   hhua ኋ   hhuie ኌ"],
  ["k", "kue ኰ   kuu ኵ   kui ኲ   kua ኳ   kuie ኴ"],
  ["K", "Kue ዀ   Kuu ዅ   Kui ዂ   Kua ዃ   Kuie ዄ"],
  ["g", "gue ጐ   guu ጕ   gui ጒ   gua ጓ   guie ጔ"],
];

const cell: CSSProperties = { border: "1px solid #d6d6d6", padding: "6px 10px", textAlign: "center" };
const head: CSSProperties = { ...cell, background: "#f2f2f2", fontWeight: 600 };

function normalize(value: Settings): Settings {
  return {
    layout: value.layout === "traditional" ? "traditional" : "phonetic",
    show_typing_test: Boolean(value.show_typing_test),
  };
}

function App() {
  const [screen, setScreen] = useState<Screen>("home");
  const [settings, setSettings] = useState<Settings>({ layout: "phonetic", show_typing_test: true });
  const [status, setStatus] = useState("");
  const [testText, setTestText] = useState("");
  const [keyboard, setKeyboard] = useState<KeyboardStatus | null>(null);
  const [busy, setBusy] = useState(false);

  useEffect(() => {
    let cancelled = false;
    (async () => {
      try {
        const loaded = await invoke<Settings>("load_settings");
        if (!cancelled) setSettings(normalize(loaded));
      } catch {
        if (!cancelled) setStatus("Browser preview: settings persistence requires the desktop app.");
      }
      try {
        const state = await invoke<KeyboardStatus>("keyboard_status");
        if (!cancelled) setKeyboard(state);
      } catch {
        if (!cancelled) setKeyboard(null);
      }
    })();
    return () => {
      cancelled = true;
    };
  }, []);

  const save = async () => {
    try {
      await invoke("save_settings", { settings });
      setStatus("Settings saved.");
    } catch (error) {
      setStatus("Could not save settings: " + String(error));
    }
  };

  const openLanguageSettings = async () => {
    try {
      await invoke("open_language_settings");
    } catch (error) {
      setStatus("Could not open Windows settings: " + String(error));
    }
  };

  const installKeyboard = async () => {
    setBusy(true);
    setStatus("Waiting for administrator approval...");
    try {
      await invoke("install_keyboard");
      setKeyboard(await invoke<KeyboardStatus>("keyboard_status"));
      setStatus("Keyboard installed. Press Win+Space and choose FidelKey Amharic (Phonetic).");
    } catch (error) {
      setStatus("Could not enable the keyboard: " + String(error));
    } finally {
      setBusy(false);
    }
  };

  const removeKeyboard = async () => {
    setBusy(true);
    setStatus("Removing the keyboard...");
    try {
      await invoke("uninstall_keyboard");
      setKeyboard(await invoke<KeyboardStatus>("keyboard_status"));
      setStatus("Keyboard removed. If it still appears in the language list, sign out and back in.");
    } catch (error) {
      setStatus("Could not remove the keyboard: " + String(error));
    } finally {
      setBusy(false);
    }
  };

  const keyboardButtonStyle = {
    padding: "10px 16px",
    borderRadius: 6,
    border: "1px solid #1f6feb",
    background: "#1f6feb",
    color: "#fff",
    cursor: busy ? "default" : "pointer",
    opacity: busy ? 0.7 : 1,
  } as CSSProperties;

  return (
    <div style={{ fontFamily: "Segoe UI, Arial, sans-serif", maxWidth: 900, margin: "0 auto", padding: 24, color: "#1c1c1c" }}>
      <nav style={{ display: "flex", gap: 8, marginBottom: 24, borderBottom: "1px solid #ddd", paddingBottom: 12 }}>
        {(["home", "settings", "reference", "about"] as Screen[]).map((item) => (
          <button
            key={item}
            onClick={() => setScreen(item)}
            style={{
              padding: "8px 14px",
              borderRadius: 6,
              border: "1px solid #ccc",
              background: screen === item ? "#1f6feb" : "#fff",
              color: screen === item ? "#fff" : "#1c1c1c",
              cursor: "pointer",
              textTransform: "capitalize",
            }}
          >
            {item === "reference" ? "Keyboard Reference" : item}
          </button>
        ))}
      </nav>

      {status && (
        <div style={{ marginBottom: 16, padding: "10px 12px", background: "#eef4ff", border: "1px solid #c7d8ff", borderRadius: 6 }}>
          {status}
        </div>
      )}

      {screen === "home" && (
        <div>
          <h1 style={{ marginTop: 0 }}>FidelKey</h1>
          <p>Amharic phonetic keyboard for Windows 10 and 11. Free, offline, ad-free, no telemetry.</p>
          <p style={{ color: "#666" }}>Made by Afbix.</p>
          <p><strong>Current layout:</strong> Phonetic (Latin to Amharic)</p>

          <h2>Setup</h2>

          {keyboard && keyboard.packaged && !keyboard.installed && (
            <div style={{ marginBottom: 16, padding: "12px 14px", background: "#fff8e6", border: "1px solid #e8c56b", borderRadius: 6 }}>
              <p style={{ marginTop: 0 }}>
                The Store version needs one administrator approval to register the system keyboard.
              </p>
              <button onClick={installKeyboard} disabled={busy} style={keyboardButtonStyle}>
                {busy ? "Waiting for approval..." : "Enable FidelKey keyboard"}
              </button>
            </div>
          )}

          {keyboard && keyboard.packaged && keyboard.installed && (
            <p>
              <strong>Keyboard installed.</strong>{" "}
              <button
                onClick={removeKeyboard}
                disabled={busy}
                style={{ ...keyboardButtonStyle, background: "#fff", color: "#1c1c1c", border: "1px solid #ccc" }}
              >
                {busy ? "Removing..." : "Remove keyboard"}
              </button>
            </p>
          )}

          {keyboard && !keyboard.packaged && (
            <p>
              {keyboard.installed
                ? "The keyboard was registered by the FidelKey installer."
                : "Run the FidelKey installer to register the keyboard."}
            </p>
          )}

          <ol>
            {(!keyboard || !keyboard.packaged || keyboard.installed) ? (
              <li>Press <kbd>Win</kbd> + <kbd>Space</kbd> and select <strong>FidelKey Amharic (Phonetic)</strong>.</li>
            ) : (
              <li>After enabling the keyboard, press <kbd>Win</kbd> + <kbd>Space</kbd> and select <strong>FidelKey Amharic (Phonetic)</strong>.</li>
            )}
            <li>Type Latin letters and they become Fidel, for example <code>selam</code> becomes ሰላም.</li>
          </ol>
          <button onClick={openLanguageSettings} style={{ padding: "10px 16px", borderRadius: 6, border: "1px solid #1f6feb", background: "#1f6feb", color: "#fff", cursor: "pointer" }}>
            Open Windows language settings
          </button>

          {settings.show_typing_test && (
            <div style={{ marginTop: 24 }}>
              <h2>Practice area</h2>
              <p style={{ color: "#666" }}>With the FidelKey keyboard selected, type here to try it out.</p>
              <textarea
                value={testText}
                onChange={(event) => setTestText(event.target.value)}
                placeholder="Type here..."
                style={{ width: "100%", height: 120, padding: 10, fontSize: 16, borderRadius: 6, border: "1px solid #ccc", boxSizing: "border-box" }}
              />
            </div>
          )}
        </div>
      )}

      {screen === "settings" && (
        <div>
          <h1 style={{ marginTop: 0 }}>Settings</h1>

          <h2>Layout</h2>
          <label style={{ display: "block", marginBottom: 10 }}>
            <input
              type="radio"
              name="layout"
              checked={settings.layout === "phonetic"}
              onChange={() => setSettings({ ...settings, layout: "phonetic" })}
            />{" "}
            Phonetic (Latin to Amharic) — default
          </label>
          <label style={{ display: "block", marginBottom: 10, color: "#888" }}>
            <input type="radio" name="layout" disabled /> Traditional Fidel layout (planned, not yet available)
          </label>

          <h2>Interface</h2>
          <label style={{ display: "block", marginBottom: 10 }}>
            <input
              type="checkbox"
              checked={settings.show_typing_test}
              onChange={(event) => setSettings({ ...settings, show_typing_test: event.target.checked })}
            />{" "}
            Show the practice area on the Home screen
          </label>

          <h2>Keyboard</h2>
          {keyboard && keyboard.packaged ? (
            <div style={{ marginBottom: 10 }}>
              <p style={{ marginTop: 0 }}>
                {keyboard.installed
                  ? "The FidelKey system keyboard is installed."
                  : "The Store package needs one administrator approval to register the system keyboard."}
              </p>
              <button
                onClick={keyboard.installed ? removeKeyboard : installKeyboard}
                disabled={busy}
                style={{
                  ...keyboardButtonStyle,
                  background: keyboard.installed ? "#fff" : "#1f6feb",
                  color: keyboard.installed ? "#1c1c1c" : "#fff",
                  border: keyboard.installed ? "1px solid #ccc" : "1px solid #1f6feb",
                }}
              >
                {busy ? "Working..." : keyboard.installed ? "Remove keyboard" : "Enable FidelKey keyboard"}
              </button>
            </div>
          ) : (
            <p style={{ color: "#666" }}>
              The keyboard is registered by the FidelKey installer. Use its uninstall entry to remove it.
            </p>
          )}

          <div style={{ display: "flex", gap: 10, marginTop: 20 }}>
            <button onClick={save} style={{ padding: "10px 20px", borderRadius: 6, border: "1px solid #1f6feb", background: "#1f6feb", color: "#fff", cursor: "pointer" }}>
              Save settings
            </button>
            <button onClick={openLanguageSettings} style={{ padding: "10px 20px", borderRadius: 6, border: "1px solid #ccc", background: "#fff", cursor: "pointer" }}>
              Open language settings
            </button>
          </div>

          <p style={{ marginTop: 20, color: "#666" }}>
            Settings are stored in your user profile. Switching input methods is done with Win+Space.
          </p>
        </div>
      )}

      {screen === "reference" && (
        <div>
          <h1 style={{ marginTop: 0 }}>Keyboard Reference</h1>
          <p>
            Each consonant is typed as a base key plus a vowel: <code>e</code>, <code>u</code>, <code>i</code>,{" "}
            <code>a</code>, <code>ie</code>, no vowel (6th order), or <code>o</code>. <code>ee</code> is accepted
            in place of <code>ie</code>.
          </p>

          <h2>Vowels</h2>
          <p><code>a</code> አ · <code>u</code> ኡ · <code>i</code> ኢ · <code>aa</code> ኣ · <code>ie</code>/<code>ee</code> ኤ · <code>e</code> እ · <code>o</code> ኦ</p>

          <h2>Consonants</h2>
          <div style={{ overflowX: "auto" }}>
            <table style={{ borderCollapse: "collapse", width: "100%", fontSize: 14 }}>
              <thead>
                <tr>
                  <th style={head}>Base</th>
                  {VOWEL_HEADER.map((v, index) => (
                    <th key={`${v}-${index}`} style={head}>{v}</th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {FAMILIES.map((family) => (
                  <tr key={family.key}>
                    <td style={{ ...cell, textAlign: "left", fontFamily: "Consolas, monospace" }}>{family.key}</td>
                    {family.forms.map((form, index) => (
                      <td key={`${family.key}-${index}`} style={{ ...cell, fontSize: 18 }}>{form}</td>
                    ))}
                  </tr>
                ))}
              </tbody>
            </table>
          </div>

          <h2>Labiovelars</h2>
          <p>Add <code>u</code> before the vowel.</p>
          <table style={{ borderCollapse: "collapse", width: "100%", fontSize: 14 }}>
            <tbody>
              {LABIOVELARS.map(([family, forms]) => (
                <tr key={family}>
                  <td style={{ ...cell, textAlign: "left", fontFamily: "Consolas, monospace", width: 80 }}>{family}</td>
                  <td style={{ ...cell, textAlign: "left" }}>{forms}</td>
                </tr>
              ))}
            </tbody>
          </table>

          <h2>Examples</h2>
          <ul>
            <li><code>selam</code> → ሰላም</li>
            <li><code>amen</code> → አመን</li>
            <li><code>abeba</code> → አበባ</li>
            <li><code>ityoPya</code> → ኢትዮጵያ</li>
            <li><code>Syon</code> → ጽዮን</li>
          </ul>

          <p style={{ color: "#666" }}>
            Convenience aliases: <code>sh</code> for <code>x</code> (ሸ) and <code>ch</code> for <code>c</code> (ቸ).
          </p>
        </div>
      )}

      {screen === "about" && (
        <div>
          <h1 style={{ marginTop: 0 }}>About FidelKey</h1>
          <p>FidelKey is a free, ad-free Amharic keyboard for Windows 10 and 11.</p>
          <ul>
            <li>No telemetry and no keylogging</li>
            <li>No ads and no accounts</li>
            <li>Works offline</li>
          </ul>
          <p>Typing data never leaves your device. The settings app only reads and writes a local JSON file.</p>
          <p>Made by <strong>Afbix</strong>.</p>
          <p style={{ color: "#666" }}>© 2026 Afbix. All rights reserved.</p>
        </div>
      )}
    </div>
  );
}

export default App;
