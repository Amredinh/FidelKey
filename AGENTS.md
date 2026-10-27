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
- **Status:** Milestone 5/6 - all CI jobs green, installer built; runtime testing pending
- **Phase:** Phonetic engine, TSF service, settings app and NSIS installer compile and package in CI
- **Build State:** Verified on 2026-10-26 by CI run `40274034101`: engine tests, TSF DLL, settings app and `FidelKey-Setup-0.1.0.exe` all succeeded.

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
- [x] Set the Microsoft Store package identity to `afbix.fidelkey` and the
      publisher to `Afbix` (Tauri identifier and `bundle.publisher`; installer
      CompanyName/Publisher updated to match)

### 2026-10-27
- [x] Imported the final logo, removed the corner "K" watermark, and rebuilt a
      clean 1024 px master (`assets/branding/`)
- [x] Generated the Windows icon set (16-256 px ICO and PNGs) and Store 1:1
      exports (300/150/71/1080 px)
- [x] Wired the TSF language profile to use the installed `FidelKey.exe` icon
- [ ] MSIX output: Tauri emits EXE/MSI only; a pure MSIX cannot register a
      machine-wide TSF TIP reliably. The Store EXE/MSI path is the supported
      route (signed installer, silent `/S`, offline WebView2).

## Milestones
1. **Milestone 1 — Mapping specification** (DONE, GFF 3.3)
2. **Milestone 2 — Conversion engine** (DONE, engine tests pass in CI)
3. **Milestone 3 — Windows integration (TSF)** (COMPILES IN CI, runtime untested)
4. **Milestone 4 — Settings interface (Tauri)** (DONE, builds in CI)
5. **Milestone 5 — Installer** (DONE, builds in CI)
6. **Milestone 6 — Testing and release** (IN PROGRESS, manual testing only)

## Current Focus
- Install the CI-built setup on a real Windows machine and test Notepad first
- Verify TSF activation, composition, backspace, punctuation and uninstall
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
- Final logo is in place; verify it at 16/24/32 px and in the language bar.
- Password/secure input contexts are not specially filtered yet.

## Next Actions
1. Download the `FidelKey-Setup` artifact and install it on Windows 10/11.
2. Verify Win+Space activation and typing in Notepad, Word, and a browser.
3. Verify uninstall removes the profile, files, and shortcuts cleanly.
4. Verify the engine test job genuinely fails on a broken mapping.
5. Address runtime TSF issues found during testing.
6. Verify the new icons and the TSF language-bar icon.
7. Resolve Store packaging: use the EXE/MSI route and drop MSIX for the keyboard.
8. Add an offline WebView2 installer, sign the setup, test `/S`, then submit the
   Win32 installer through Partner Center.
9. Implement the traditional layout or explicitly drop it.
