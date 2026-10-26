import React, { useState } from "react";

type Screen = "home" | "settings" | "reference" | "about";

function App() {
  const [screen, setScreen] = useState<Screen>("home");
  const [layout, setLayout] = useState<"phonetic" | "traditional">("phonetic");
  const [testText, setTestText] = useState("");

  return (
    <div style={{ fontFamily: "Arial, sans-serif", maxWidth: "800px", margin: "0 auto", padding: "20px" }}>
      <nav style={{ marginBottom: "20px", borderBottom: "1px solid #ccc", paddingBottom: "10px" }}>
        <button onClick={() => setScreen("home")} style={{ marginRight: "10px" }}>Home</button>
        <button onClick={() => setScreen("settings")} style={{ marginRight: "10px" }}>Settings</button>
        <button onClick={() => setScreen("reference")} style={{ marginRight: "10px" }}>Keyboard Reference</button>
        <button onClick={() => setScreen("about")}>About</button>
      </nav>

      {screen === "home" && (
        <div>
          <h1>FidelKey</h1>
          <p>Version: 0.1.0</p>
          <p>Current layout: {layout}</p>
          <h2>Setup Instructions</h2>
          <ol>
            <li>Install FidelKey using the installer</li>
            <li>Press Win+Space to open the input method selector</li>
            <li>Select FidelKey from the list</li>
            <li>Start typing Amharic phonetically</li>
          </ol>
          <h2>Typing Test</h2>
          <textarea
            value={testText}
            onChange={(e) => setTestText(e.target.value)}
            placeholder="Type here to test..."
            style={{ width: "100%", height: "100px", fontSize: "16px" }}
          />
        </div>
      )}

      {screen === "settings" && (
        <div>
          <h1>Settings</h1>
          <h2>Layout Selection</h2>
          <label style={{ display: "block", marginBottom: "10px" }}>
            <input
              type="radio"
              name="layout"
              value="phonetic"
              checked={layout === "phonetic"}
              onChange={() => setLayout("phonetic")}
            />
            Phonetic (Latin to Amharic) - Default
          </label>
          <label style={{ display: "block", marginBottom: "10px" }}>
            <input
              type="radio"
              name="layout"
              value="traditional"
              checked={layout === "traditional"}
              onChange={() => setLayout("traditional")}
            />
            Traditional Fidel Layout
          </label>
          <button onClick={() => alert("Settings saved!")} style={{ marginTop: "20px", padding: "10px 20px" }}>
            Save Settings
          </button>
          <p style={{ marginTop: "20px", color: "#666" }}>
            After changing layout, you may need to switch input methods using Win+Space.
          </p>
        </div>
      )}

      {screen === "reference" && (
        <div>
          <h1>Keyboard Reference</h1>
          <p>Layout: {layout}</p>
          <h2>Phonetic Mapping (GFF Style)</h2>
          <table style={{ width: "100%", borderCollapse: "collapse" }}>
            <thead>
              <tr>
                <th style={{ border: "1px solid #ccc", padding: "8px" }}>Input</th>
                <th style={{ border: "1px solid #ccc", padding: "8px" }}>Output</th>
                <th style={{ border: "1px solid #ccc", padding: "8px" }}>Example</th>
              </tr>
            </thead>
            <tbody>
              <tr><td style={{ border: "1px solid #ccc", padding: "8px" }}>s, se, su, si, sa, sie, s, so</td><td style={{ border: "1px solid #ccc", padding: "8px" }}>ሰ ሱ ሲ ሳ ሴ ስ ሶ</td><td style={{ border: "1px solid #ccc", padding: "8px" }}>selam → ሰላም</td></tr>
              <tr><td style={{ border: "1px solid #ccc", padding: "8px" }}>m, me, mu, mi, ma, mie, m, mo</td><td style={{ border: "1px solid #ccc", padding: "8px" }}>መ ሙ ሚ ማ ሜ ም ሞ</td><td style={{ border: "1px solid #ccc", padding: "8px" }}>amen → አመን</td></tr>
              <tr><td style={{ border: "1px solid #ccc", padding: "8px" }}>l, le, lu, li, la, lie, l, lo</td><td style={{ border: "1px solid #ccc", padding: "8px" }}>ለ ሉ ሊ ላ ሌ ል ሎ</td><td style={{ border: "1px solid #ccc", padding: "8px" }}>lula → ሉላ</td></tr>
            </tbody>
          </table>
        </div>
      )}

      {screen === "about" && (
        <div>
          <h1>About FidelKey</h1>
          <p>FidelKey is a free, ad-free Amharic keyboard for Windows.</p>
          <h2>Privacy</h2>
          <ul>
            <li>No telemetry</li>
            <li>No keylogging</li>
            <li>No ads</li>
            <li>Works offline</li>
            <li>No accounts required</li>
          </ul>
          <h2>License</h2>
          <p>Open source - see GitHub repository for details.</p>
          <h2>Links</h2>
          <p>Source: https://github.com/Amredinh/FidelKey</p>
        </div>
      )}
    </div>
  );
}

export default App;
