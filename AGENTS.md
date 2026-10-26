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
- **Status:** Milestone 4/5 - full app implemented, awaiting CI validation
- **Phase:** Phonetic engine, TSF service, settings app and NSIS installer written
- **Build State:** NOT verified since the full rewrite. Run the workflow and fix failures.

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

## Milestones
1. **Milestone 1 — Mapping specification** (DONE, GFF 3.3)
2. **Milestone 2 — Conversion engine** (IMPLEMENTED, tests written, CI unverified)
3. **Milestone 3 — Windows integration (TSF)** (IMPLEMENTED, needs real Windows testing)
4. **Milestone 4 — Settings interface (Tauri)** (IMPLEMENTED, CI unverified)
5. **Milestone 5 — Installer** (IMPLEMENTED with NSIS, CI unverified)
6. **Milestone 6 — Testing and release** (NOT STARTED)

## Current Focus
- Push the rewrite and watch the four CI jobs
- Fix compile/link errors reported by CI
- Install the produced setup on a real Windows machine and test Notepad first

## Known Issues / Uncertainties
- The full rewrite has not been compiled yet. Treat CI as the first compiler.
- TSF registration, composition lifetime and Win+Space activation must be tested
  on a real Windows install, not just compiled.
- Traditional Fidel physical layout is documented but not implemented.
- The layout setting is persisted by the settings app but the TSF service still
  always uses the phonetic layout (only layout available).
- No sign-out/restart handling is implemented for newly registered profiles.
- Installer is unsigned; SmartScreen will warn.
- Password/secure input contexts are not specially filtered yet.

## Next Actions
1. Run the GitHub Actions workflow; fix every failure.
2. Verify the engine test job genuinely fails on a broken mapping.
3. Test the installed keyboard in Notepad, Word, and a browser.
4. Verify uninstall removes the profile and files cleanly.
5. Implement the traditional layout or explicitly drop it from the requirements.
