function App() {
  return (
    <div style={{ padding: "20px", fontFamily: "Arial, sans-serif" }}>
      <h1>FidelKey Settings</h1>
      <p>Amharic Keyboard for Windows</p>
      
      <section>
        <h2>Layout</h2>
        <label>
          <input type="radio" name="layout" value="phonetic" defaultChecked />
          Phonetic (Latin to Amharic) - Default
        </label>
        <br />
        <label>
          <input type="radio" name="layout" value="traditional" />
          Traditional Fidel Layout
        </label>
      </section>
      
      <section>
        <h2>Typing Test</h2>
        <textarea 
          placeholder="Type here to test..." 
          style={{ width: "100%", height: "100px" }}
        />
      </section>
      
      <section>
        <h2>Instructions</h2>
        <p>Press Win+Space to switch input methods.</p>
        <p>Select FidelKey from the input list.</p>
      </section>
    </div>
  );
}

export default App;
