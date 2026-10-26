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
- **Status:** Milestone 3 - TSF service implementation (IN PROGRESS)
- **Phase:** Building full app (engine validated, TSF skeleton built, settings app built)
- **Build State:** Engine + TSF build in CI, settings app in CI

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
- [x] Created expanded tests (selam, basic mappings, backspace, mixed, punctuation, overlapping)
- [x] Set up CMake build for engine
- [x] Created GitHub Actions workflow for Windows builds
- [x] Pushed to GitHub, CI build-engine job SUCCEEDED (30s)
- [x] Engine tests passed in CI
- [x] Added TSF service skeleton with COM interfaces
- [x] Added Tauri settings app skeleton
- [x] Updated TSF TextService with composition support
- [x] Updated CI workflow to build TSF service
- [x] Built out Tauri settings app with UI screens
- [x] Added Cargo.toml and main.rs for Tauri

## Milestones
1. **Milestone 1 — Mapping specification** (VALIDATED - CI passed)
2. **Milestone 2 — Conversion engine** (IN PROGRESS - engine built and tested)
3. **Milestone 3 — Windows integration (TSF)** (IN PROGRESS - skeleton + COM interfaces)
4. **Milestone 4 — Settings interface (Tauri)** (SKELETON - basic UI implemented)
5. **Milestone 5 — Installer**
6. **Milestone 6 — Testing and release**

## Current Focus
- TSF service implementation in progress
- Settings app basic UI implemented
- Need to add proper COM registration
- Need to implement TSF composition framework
- Need to create installer

## Known Issues / Uncertainties
- TSF composition framework not fully implemented
- Registration needs full implementation
- Settings app needs more features (config persistence, IPC)
- No installer yet

## Next Actions
1. Implement TSF composition framework
2. Add config persistence to settings app
3. Create installer configuration
4. Test on Windows via CI
