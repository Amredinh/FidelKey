# Build FidelKey — Free, Lightweight Amharic Keyboard for Windows

## Project Overview

Build **FidelKey**, a lightweight, completely free and ad-free Amharic keyboard application for Windows 10 and Windows 11.

The application must allow users to type Amharic across Windows applications, including Notepad, Microsoft Word, web browsers, email clients, and other compatible programs.

The project must prioritize simplicity, performance, reliability, privacy, and a small installation footprint. Do not overengineer the application.

## 1. Required Technology Stack

Use the following architecture:

- **Windows input service:** Native C++ using Microsoft's Text Services Framework (TSF), with the necessary COM interfaces.
- **Desktop settings application:** Tauri 2, React, and TypeScript.
- **Frontend styling:** Lightweight CSS. Avoid unnecessary UI libraries.
- **Configuration:** Local JSON or another lightweight local configuration format.
- **Testing:** Automated unit tests for the typing engine and Windows integration tests.
- **Repository:** GitHub with automated Windows builds using GitHub Actions.

Keep the Windows input service independent of the Tauri frontend. The keyboard must continue working when the settings window is closed.

Do not simulate typing using SendInput as a substitute for proper TSF integration. Do not make a browser-based keyboard or a standalone text editor the primary input mechanism.

## 2. Default Behavior

After installation and initial setup:

- Phonetic Amharic typing must be the default FidelKey layout.
- Users must be able to type Amharic using familiar Latin-letter combinations.
- The traditional Fidel keyboard layout must be available as an alternative.
- Users must be able to change the selected layout from Settings.
- The selected layout must persist across application restarts and Windows restarts.
- Users must be able to switch between FidelKey and other installed Windows input methods using standard Windows language and input controls.

Do not attempt to change the user's entire Windows default input language silently. Guide the user through enabling FidelKey and selecting it when necessary.

## 3. Phonetic Typing Engine

Implement a deterministic, testable phonetic conversion engine.

Requirements:

- Convert supported Latin-letter sequences into correct Ethiopic Unicode characters.
- Support the seven common Fidel vowel orders where appropriate to the chosen mapping.
- Support common Amharic consonant combinations and syllable sequences.
- Handle spaces, punctuation, digits, and mixed English-Amharic text.
- Support correct backspace behavior, including incomplete sequences.
- Handle word boundaries and text commitment predictably.
- Avoid silently converting ordinary English text when the user intends to type English.
- Document the chosen mapping and ambiguous sequences.
- Keep the conversion rules in a separate module rather than hardcoding them into the Windows integration.

Example acceptance test:

Input: `selam`

Expected output: `ሰላም`

Treat this as a required test case, not proof that the complete phonetic mapping is correct. Add a comprehensive test suite covering common words and edge cases.

Do not invent an undocumented conversion standard. Define the mapping explicitly and make it easy to improve.

## 4. Traditional Fidel Layout

Implement a second layout based on an explicitly documented physical-key mapping.

Requirements:

- Map physical keyboard keys to the appropriate Fidel characters.
- Support the relevant character orders and punctuation.
- Provide a visual keyboard reference in the settings application.
- Let users select the traditional layout without reinstalling the application.
- Keep the mapping in separate configuration data.
- Document the exact layout used and avoid claiming compatibility with an established layout unless it has been verified.

The traditional layout must not replace phonetic typing as the default.

## 5. Native Windows Integration

Implement the native TSF service so that FidelKey works system-wide in compatible Windows applications.

Requirements:

- Register and activate the input service using supported Windows mechanisms.
- Integrate correctly with the Windows input method selector.
- Support text composition, commitment, and cancellation.
- Handle cursor movement, selection, and backspace correctly.
- Preserve existing text and avoid inserting duplicated characters.
- Support switching between English and Amharic.
- Handle application focus changes safely.
- Avoid interfering with password fields or other sensitive input contexts where the operating system exposes that information.
- Fail gracefully if the input service is unavailable.
- Implement correct installation, registration, upgrade, and uninstallation behavior.

Test the implementation with Notepad first, then test Word, browsers, and other compatible applications.

Do not claim universal compatibility until it has been tested.

## 6. Lightweight Settings Application

Build a simple, clean Tauri 2 desktop application.

Include only these essential screens:

### Home
- FidelKey name and version.
- Current selected layout.
- Keyboard activation and setup instructions.
- A simple typing test area.

### Settings
- Phonetic typing selected by default.
- Option to switch to the traditional Fidel layout.
- Save the user's preference locally.
- Clear instructions for applying the selected layout.

### Keyboard Reference
- Display the selected layout.
- Show example keystrokes and their corresponding Fidel characters.

### About and Help
- Installation instructions.
- Troubleshooting guidance.
- Project version and license.
- Link to the public source repository.

Keep the interface minimal and responsive. Avoid dashboards, animations, accounts, unnecessary settings, and large dependencies.

The typing test area is for demonstration and testing; it must not replace system-wide keyboard functionality.

## 7. Unicode and Language Support

- Emit valid Ethiopic Unicode text.
- Never depend on proprietary legacy font encodings.
- Support standard Unicode-compatible fonts.
- Preserve correct text when copying and pasting.
- Test Amharic punctuation, numerals where supported, and mixed Latin/Ethiopic text.
- Do not require an internet connection to type.
- Document any known Unicode or application compatibility limitations.

## 8. Privacy and Offline Operation

FidelKey must be fully functional offline after installation.

Strict requirements:

- No advertisements.
- No telemetry.
- No key logging.
- No analytics SDKs.
- No accounts or user registration.
- No cloud processing of typed text.
- No network requests required for conversion.
- Store only necessary preferences locally.
- Do not transmit typed content under any circumstances.

Updates may be downloaded separately from the official release channel. Do not implement an automatic updater unless it is necessary, secure, and explicitly documented.

## 9. Build and Distribution

Use GitHub Actions for automated Windows builds.

The workflow must:

- Run on a Windows runner.
- Install or use the required Rust, Node.js, Windows SDK, and C++ build tools.
- Build and test the native TSF service.
- Build the Tauri settings application.
- Package the application and native service correctly.
- Run available automated tests.
- Produce versioned release artifacts.
- Report build failures clearly.

Design the installer to install the native input service and settings application together. Ensure the native service is correctly registered and available to Windows.

Investigate suitable MSIX or other Windows installer packaging. Verify the applicable signing, registration, and compatibility requirements before selecting the final distribution format.

Publish downloadable releases on GitHub. Keep the application free and ad-free.

Do not assume that a successful Tauri build automatically includes a working TSF service. Verify the complete installation on a clean Windows machine.

## 10. Performance Requirements

- Keep the application small and responsive.
- Minimize dependencies and background processes.
- Avoid unnecessary memory usage.
- Keep the typing engine fast and deterministic.
- Do not run heavy services when they are not needed.
- Keep the settings application separate from the core input service.
- Avoid bundling unnecessary fonts, dictionaries, or large assets.

Measure actual installation size and memory usage rather than making unsupported performance claims.

## 11. Development Milestones

Work incrementally in this order:

1. Inspect the repository and establish the build environment.
2. Define the phonetic mapping and traditional layout.
3. Implement and test the standalone conversion engine.
4. Build the native Windows TSF input service.
5. Verify typing in Notepad and additional applications.
6. Build the Tauri settings application.
7. Connect settings to the native service using a secure, documented configuration mechanism.
8. Implement installation, registration, upgrades, and uninstallation.
9. Configure GitHub Actions and generate Windows release artifacts.
10. Test the complete installation on a clean Windows system.
11. Document setup, limitations, and release procedures.

Each milestone must leave the project in a buildable state.

## 12. Engineering Rules

- Inspect the existing repository before changing files.
- Follow existing project conventions where reasonable.
- Do not replace working code without a clear reason.
- Do not leave placeholder implementations for essential features.
- Do not claim features are complete unless they are implemented and tested.
- Avoid unnecessary abstractions and dependencies.
- Keep the native input service and conversion engine independently testable.
- Use clear names, small modules, and meaningful error messages.
- Never commit credentials, signing certificates, or private keys.
- Use GitHub Actions secrets for sensitive build credentials when required.
- Document all manual setup steps and external build dependencies.

If a Windows API limitation or packaging issue arises, research the supported solution before implementing a workaround.

## 13. Definition of Done

The project is ready for its first public release only when:

- Phonetic typing works and is the default.
- Traditional Fidel mapping is available and selectable in Settings.
- Both layouts produce correct Unicode output according to documented mappings.
- The keyboard works in the tested Windows applications.
- Users can switch between input methods.
- The application works offline.
- The installer registers the input service correctly.
- Uninstallation removes the application cleanly.
- GitHub Actions produces a usable Windows release.
- Automated tests pass.
- No ads, telemetry, or key logging are present.
- Installation and troubleshooting documentation is complete.

## Final Instruction

Start by examining the repository, checking the available Windows build tools, and proposing a concise implementation plan. Then implement the project milestone by milestone.

Prioritize a genuinely functional Windows keyboard over visual polish. Keep FidelKey lightweight, reliable, private, free, and easy to install.

Do not stop after generating a project scaffold. Verify the actual native input behavior and the complete installation process before declaring the MVP complete.