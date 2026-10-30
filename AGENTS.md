# AGENTS.md - FidelKey Development Tracking

This file tracks all development activities for the FidelKey project.

## Project Overview
FidelKey is a lightweight, free, ad-free Amharic keyboard for Windows 10/11 using TSF for system-wide input. Architecture: Native C++ TSF service + Tauri 2/React/TypeScript settings app.

## Development Rules
- **DO NOT** install C++ workloads, Windows SDKs, compilers, or large dependencies locally
- **DO NOT** run local builds/tests requiring these tools
- All builds, compilation, tests go through GitHub Actions on Windows runner
- Write code, document requirements, fix based on user feedback
- User will commit/push and test builds themselves
- **DO NOT** commit/push on behalf of user
- **DO NOT** make significant local environment changes without permission

## Status
- **Start Date:** 2026-10-24
- **Status:** Milestone 6 - MSIX display name corrected; green rebuild ready to re-upload
- **Phase:** Phonetic engine, TSF service, settings app, NSIS installer and MSIX package pipeline
- **Build State:** Verified on 2026-10-30 by CI run `41057312871`: all five jobs green, including `FidelKey_0.1.0_x64.msix` (2.47 MB, 46 packaged files) with identity `Afbix.FidelKeyAmharicKeyboard`, publisher `CN=1DE27A23-6791-4654-8220-956F0BBED2A8`, display name `FidelKey Amharic Keyboard`, and `FidelKey-Setup-0.1.0.exe`.

## Activity Log

### 2026-10-24
- [x] Read BUILD.md and updated requirements
- [x] Explored repo - only BUILD.md existed
- [x] Initialized git repository
- [x] Created AGENTS.md for tracking
- [x] Created project structure
- [x] Researched GFF Amharic layout patterns
- [x] Created mapping specification document
- [x] Designed conversion engine API
- [x] Implemented trie-based prefix matching
- [x] Implemented mapping table with GFF-style mappings
- [x] Implemented PhoneticEngine with composition logic
- [x] Set up CMake build for engine
- [x] Created GitHub Actions workflow for Windows builds
- [x] Added TSF service skeleton with COM interfaces
- [x] Added Tauri settings app skeleton
- [x] Built out Tauri settings app UI screens

### 2026-10-25
- [x] Audited existing code and found the Release build compiled out all asserts
- [x] Corrected the mapping table against GFF 3.3 (`x`=ሸ, `hh`=ኀ, `H`=ሐ,
      `ss`=ሠ, `S`=ጸ, `SS`=ፀ, `N`=ኘ, `T`=ጠ, `C`=ጨ, `K`=ኸ, `P`=ጰ, glottal vowels)
- [x] Removed duplicate `tse` mapping and added labiovelar `ue/uu/ui/ua/uie`
- [x] Replaced `assert` tests with a Release-safe harness and exhaustive table
      coverage (all families, orders, labiovelars, aliases, duplicates)
- [x] Refactored engine to greedy whole-word conversion, removing mid-word commits
- [x] Implemented real TSF composition via `ITfEditSession`, `ITfComposition`,
      `ITfKeyEventSink` and `ITfCompositionSink`
- [x] Implemented real COM + TSF registration (`ITfInputProcessorProfiles`,
      `ITfCategoryMgr`, Amharic lang id `0x045E`)
- [x] Fixed Tauri 2 scaffold (vite config, tsconfig, `src/main.rs`, `build.rs`,
      capabilities, icon, no-bundle build) and added settings persistence commands
- [x] Added root CMake build, UTF-8 compile flag and static MSVC runtime
- [x] Added NSIS installer (`installer/fidelkey.nsi`) and installer CI job
- [x] Rewrote the GitHub Actions workflow (engine, TSF, settings, installer)

### 2026-10-26
- [x] Validated the mapping independently: 355 entries, no duplicate keys, all 317
      explicit expectations and every sample word match the table
- [x] Fixed a test that wrongly expected `ABC` to pass through (`C` maps to ጭ)
- [x] CI green for the engine tests and the Tauri settings app
- [x] Fixed TSF compilation: start compositions with
      `ITfContextComposition::StartComposition` and a `TF_SELECTION` range
- [x] Fixed TSF exports: use a `.def` file and `__stdcall`-compatible signatures;
      dropped the nonexistent `msctf.lib` (TSF is consumed purely through COM)
- [x] Fixed NSIS: move `SetShellVarContext`/`SetRegView` into sections and
      register the x64 DLL via `Sysnative\regsvr32.exe` from the 32-bit stub
- [x] Confirmed the full pipeline produces `FidelKey-Setup-0.1.0.exe` (25.5 MB,
      includes the VC++ redistributable)
- [x] Set the initial Store package identity to `afbix.fidelkey` and the
      publisher to `Afbix` (Tauri identifier and `bundle.publisher`; installer
      CompanyName/Publisher updated to match); replaced on 2026-10-29 with the
      reserved Partner Center identity

### 2026-10-27
- [x] Imported the final logo, removed the corner "K" watermark, and rebuilt a
      clean 1024 px master (`assets/branding/`)
- [x] Generated the Windows icon set (16-256 px ICO and PNGs) and Store 1:1
      exports (300/150/71/1080 px)
- [x] Wired the TSF language profile to use the installed `FidelKey.exe` icon
- [x] CI run `40570837189` green; installer rebuilt with the new icons
- [x] Added best-effort MSIX packaging: `installer/msix/AppxManifest.xml`,
      generated tile/Store/splash assets, and a `msix` CI job using `MakeAppx`
- [x] Added first-run elevation path in the settings app: `--install-tsf` copies
      the DLL/exe to `C:\Program Files\FidelKey` and calls `regsvr32`, with a
      matching `--uninstall-tsf`; the app shows Enable/Remove keyboard controls
- [x] Fixed the MSIX manifest: a Win32 app with
      `uap10:RuntimeBehavior="win32App"` must not declare `EntryPoint`
- [x] CI run `41011840071` green; MSIX artifact produced and unpacked for
      inspection: Identity `afbix.fidelkey` 0.1.0.0 x64, `mediumIL`/`win32App`,
      `runFullTrust` + `allowElevation`, `FidelKey.exe`, `FidelKeyTSF.dll`,
      44 files total
- [ ] Install the MSIX on real Windows and test the first-run UAC registration
- [ ] Store submission: complete the Partner Center listing; the
      `allowElevation` capability and system-wide TSF install may be rejected

### 2026-10-29
- [x] Matched the reserved Partner Center identity:
      `Afbix.FidelKeyAmharicKeyboard`, publisher
      `CN=1DE27A23-6791-4654-8220-956F0BBED2A8`; updated the MSIX manifest,
      CI fallback and MSIX README
- [x] Removed the GitHub link from the settings app About screen and added
      "Made by Afbix" / Afbix copyright; no external website link in the app UI
- [x] Created local-only `store.md` with English + Amharic Store listing copy,
      age-rating answers, package notes and certification notes; added it to
      `.gitignore` so it is never pushed
- [x] CI run `41014959376` green after the identity change; downloaded and
      inspected `FidelKey_0.1.0_x64.msix`: Identity
      `Afbix.FidelKeyAmharicKeyboard`, publisher
      `CN=1DE27A23-6791-4654-8220-956F0BBED2A8`, version 0.1.0.0, x64, 46 files
- [ ] Test the MSIX (Enable, typing, Remove) on a real Windows machine
- [ ] Upload the MSIX and complete the English + Amharic Store listing

### 2026-10-30
- [x] Store validation rejected the package:
      `Package/Properties/DisplayName` was `FidelKey`, which is not a reserved
      name. The reserved product name is `FidelKey Amharic Keyboard`.
- [x] Changed both manifest display names to `FidelKey Amharic Keyboard`; the
      package identity and publisher remain correct
- [x] CI run `41057312871` green after the display-name change; downloaded and
      inspected the new MSIX: `FidelKey Amharic Keyboard`, identity and publisher
      unchanged, 46 packaged files
- [ ] Re-upload the new MSIX to Partner Center
- [ ] Continue runtime testing after the package is accepted

## Milestones
1. **Milestone 1 — Mapping specification** (DONE, GFF 3.3)
2. **Milestone 2 — Conversion engine** (DONE, engine tests pass in CI)
3. **Milestone 3 — Windows integration (TSF)** (COMPILES IN CI, runtime untested)
4. **Milestone 4 — Settings interface (Tauri)** (DONE, builds in CI)
5. **Milestone 5 — Installer** (DONE, builds in CI)
6. **Milestone 6 — Testing and release** (IN PROGRESS, manual testing only)

## Current Focus
- Rebuild the MSIX with the Partner Center identity and verify the package
- Test the MSIX Enable/Remove flow and TSF typing on a real Windows machine
- Prepare the English + Amharic Store listing from the local `store.md`
- Decide whether to implement the traditional layout before release

## Known Issues / Uncertainties
- TSF logic compiles but has never run. Boundary keys are finalized from
  `OnTestKeyDown`, which may need revisiting during real-app testing.
- TSF registration is per-machine and writes HKLM; installation needs admin.
- Newly registered profiles may not appear until sign-out or a language-bar
  restart; no explicit handling is implemented.
- Traditional Fidel physical layout is documented but not implemented.
- The layout setting is persisted by the settings app but the TSF service still
  always uses the phonetic layout (only layout available).
- Installer is unsigned; SmartScreen will warn. The installer stub is 32-bit and
  registers the x64 DLL through `Sysnative`.
- Microsoft Store path: Tauri produces EXE/MSI, not MSIX; Partner Center's
  Win32 app flow requires a code-signed, silent (`/S`) installer and an offline
  WebView2 installer. The current custom setup is neither signed nor silent-tested.
- MSIX is not a viable package for the keyboard itself: Tauri does not produce
  MSIX, and MSIX has no TSF profile/category registration extension. TSF needs
  the classic elevated installer (COM + `ITfInputProcessorProfiles` registration).
- The MSIX attempt packages a full-trust settings app plus a first-run elevated
  helper; it is not a pure MSIX TSF registration and may fail Store certification.
  The package name is `Afbix.FidelKeyAmharicKeyboard`, publisher
  `CN=1DE27A23-6791-4654-8220-956F0BBED2A8`, and display name
  `FidelKey Amharic Keyboard`; all three are fixed in the manifest.
  The display name must match a name reserved in Partner Center exactly.
- Store listing copy, IARC answers and certification notes are in the local
  `store.md`; it is intentionally gitignored and must not be pushed.
- The MSIX does not clean up `C:\Program Files\FidelKey` if the package is
  uninstalled without using the app's Remove keyboard control.
- Final logo is in place; verify it at 16/24/32 px and in the language bar.
- Password/secure input contexts are not specially filtered yet.

## Next Actions
1. Download the `FidelKey-Setup` artifact and install it on Windows 10/11.
2. Verify Win+Space activation and typing in Notepad, Word, and a browser.
3. Verify uninstall removes the profile, files, and shortcuts cleanly.
4. Verify the engine test job genuinely fails on a broken mapping.
5. Address runtime TSF issues found during testing.
6. Verify the new icons and the TSF language-bar icon.
7. Re-upload the rebuilt `FidelKey-MSIX` artifact to Partner Center; its
   identity, publisher and display name now match the reservation. Then test
   the first-run UAC registration and assess Store certification risk.
8. If MSIX is rejected, fall back to the EXE/MSI route: add an offline WebView2
   installer, sign the setup, test `/S`, then submit through Partner Center.
9. Implement the traditional layout or explicitly drop it.
